#include "search.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "document.h" // Include document.h
#include "documents_list.h" // Include documents_list.h

// Helper function to normalize a word (lowercase, remove punctuation)
char *normalizeWord(const char *word) {
    if (!word) return NULL;
    size_t len = strlen(word);
    char *normalized = malloc(len + 1); // Allocate enough memory
    if (!normalized) {
        perror("Failed to allocate memory in normalizeWord");
        return NULL;
    }
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            normalized[j++] = tolower((unsigned char)word[i]);
        }
    }
    normalized[j] = '\0';
    return normalized;
}

// Helper function to extract document snippet
char *extractSnippet(const char *text, int word_position) {
    int start = word_position - 50;
    int end = word_position + 50;

    if (start < 0) start = 0;
    if (end > (int)strlen(text)) end = (int)strlen(text);

    int actual_length = end - start;
    if (actual_length <= 0) return NULL;
    char *snippet = malloc(actual_length + 1);
    if (!snippet) return NULL;
    strncpy(snippet, text + start, actual_length);
    snippet[actual_length] = '\0';
    return snippet;
}

// Searches for documents using the reverse index
void searchDocuments(HashMap *index, QueryNode *query) {
    if (!index || !query) return;

    HashSet *results = NULL; // Use a HashSet to store unique document IDs
    int count = 0;

    QueryNode *current = query;
    while (current) {
        char *normalizedWord = normalizeWord(current->word);
        if (!normalizedWord) {
            current = current->next;
            continue; // Skip this word if normalization failed.
        }

        // 1.  Short and Full Barrel Search:
        HashSet *docIds = NULL;
        if (searchHashMap(index, normalizedWord, (void **)&docIds)) { // Search in main index
             if (!current->is_excluded) {
                if (!results) results = createHashSet();
                 for(size_t i = 0; i < docIds->size; i++){
                    if(docIds->elements[i] != NULL){
                         insertHashSet(results, docIds->elements[i]);
                    }
                 }
             }
        }

        free(normalizedWord);
        current = current->next;
    }

    if (results && results->size > 0)
    {
        printf("Matching Documents:\n");
        for (size_t i = 0; i < results->capacity; i++)
        {
            if (results->elements[i] != NULL)
            {
                Document *doc = (Document *)results->elements[i];
                printf("(%d) %s\n", doc->id, doc->title);
                // Extract and print snippet
                char *snippet = extractSnippet(doc->body, 10); // Example: Use offset 10
                if (snippet)
                {
                    printf("Snippet: ...%s...\n", snippet);
                    free(snippet);
                }
                printf("---\n");
                count++;
                if (count >= 5)
                    break;
            }
        }
    }
    else
    {
        printf("No matching documents found.\n");
    }
    freeHashSet(results);
}

// Function to build the reverse index
HashMap *buildReverseIndex(DocumentNode *docs) {
    HashMap *index = createHashMap(1000); // Start with a reasonable initial size
    if (!index) return NULL;

    while (docs) {
        // Normalize title and body, and add words to index
        char *title = strdup(docs->doc->title);
        char *body = strdup(docs->doc->body);
        if(!title || !body){
            freeHashMap(index);
            return NULL;
        }
        char *title_token = strtok(title, " \n\t.,;!?()[]{}");
        char *body_token = strtok(body, " \n\t.,;!?()[]{}");

        // Process title words
        while (title_token) {
            char *normalizedWord = normalizeWord(title_token);
            if (normalizedWord) {
                HashSet *docIds = NULL;
                if (searchHashMap(index, normalizedWord, (void **)&docIds)) {
                    insertHashSet(docIds, docs->doc);
                } else {
                    docIds = createHashSet();
                    insertHashSet(docIds, docs->doc);
                    insertHashMap(index, normalizedWord, docIds);
                }
                free(normalizedWord);
            }
            title_token = strtok(NULL, " \n\t.,;!?()[]{}");
        }

        // Process body words
        while (body_token) {
            char *normalizedWord = normalizeWord(body_token);
            if (normalizedWord) {
                HashSet *docIds = NULL;
                if (searchHashMap(index, normalizedWord, (void **)&docIds)) {
                    insertHashSet(docIds, docs->doc);
                } else {
                    docIds = createHashSet();
                    insertHashSet(docIds, docs->doc);
                    insertHashMap(index, normalizedWord, docIds);
                }
                free(normalizedWord);
            }
            body_token = strtok(NULL, " \n\t.,;!?()[]{}");
        }
        free(title);
        free(body);
        docs = docs->next;
    }
    return index;
}

// Function to serialize the reverse index to a file
void serializeReverseIndex(HashMap *index, const char *filename) {
    if (!index || !filename) return;
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file for serialization");
        return;
    }

    // Write the number of entries
    size_t numEntries = index->size;
    fwrite(&numEntries, sizeof(size_t), 1, file);

    // Iterate through the hash table
    for (size_t i = 0; i < index->capacity; i++) {
        if (index->keys[i] != NULL) {
            // Write the key (word)
            size_t keyLength = strlen(index->keys[i]) + 1; // Include null terminator
            fwrite(&keyLength, sizeof(size_t), 1, file);
            fwrite(index->keys[i], sizeof(char), keyLength, file);

            // Write the HashSet of Document pointers
            HashSet *docSet = (HashSet *)index->values[i];
            size_t setSize = docSet->size;
            fwrite(&setSize, sizeof(size_t), 1, file); // Write number of documents

            for (size_t j = 0; j < docSet->capacity; j++)
            {
                if(docSet->elements[j] != NULL){
                    Document* doc = (Document*)docSet->elements[j];
                    fwrite(&doc->id, sizeof(int), 1, file);
                }
            }
        }
    }
    fclose(file);
}

// Function to deserialize the reverse index from a file
HashMap *deserializeReverseIndex(const char *filename) {
    if (!filename) return NULL;
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file for deserialization");
        return NULL;
    }

    // Read the number of entries
    size_t numEntries;
    fread(&numEntries, sizeof(size_t), 1, file);

    // Create the hash map
    HashMap *index = createHashMap(numEntries * 2); // Create hashmap with expected size
    if (!index) {
        fclose(file);
        return NULL;
    }

    // Read each entry
    for (size_t i = 0; i < numEntries; i++) {
        // Read the key (word)
        size_t keyLength;
        fread(&keyLength, sizeof(size_t), 1, file);
        char *key = malloc(keyLength);
        if (!key) {
            perror("Failed to allocate memory for key during deserialization");
            freeHashMap(index);
            fclose(file);
            return NULL;
        }
        fread(key, sizeof(char), keyLength, file);

       // Read the HashSet
        size_t setSize;
        fread(&setSize, sizeof(size_t), 1, file);
        HashSet *docSet = createHashSet();

        for(size_t j = 0; j < setSize; j++){
            int docId;
            fread(&docId, sizeof(int), 1, file);
            // Create a dummy document.  The actual document data will need to be loaded
            // separately, and the pointers in the hash table updated.
            Document *doc = malloc(sizeof(Document));
            doc->id = docId;
            insertHashSet(docSet, doc);
        }
        insertHashMap(index, key, docSet);
        free(key);
    }
    fclose(file);
    return index;
}


void searchDocumentsLinear(DocumentNode *docs, QueryNode *query) {
    int printed = 0;
    for (DocumentNode *node = docs; node && printed < 5; node = node->next) {
        Document *d = node->doc;
        if (strcasestr(d->body, query->word)) {
            printf("(%d) %s\n", d->id, d->title);
            printf("---\n");
            printed++;
        }
    }
}

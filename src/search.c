#include "search.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "document.h"
#include "documents_list.h"
#include "graph.h" 

extern DocumentGraph *global_graph; // global graph from main

//normalize word by converting to lowercase and removing all non-alphanumeric characters
char *normalizeWord(const char *word) {
    if (!word) return NULL; //handle NULL input word
    size_t len = strlen(word); //get length of the input word
    char *normalized = malloc(len + 1); //allocate memory for the normalize word
    if (!normalized) return NULL; //check for malloc failure
    size_t j = 0; //index for the normalized word
    for (size_t i = 0; i < len; i++) { //iterate through each character of the input word.
        if (isalnum((unsigned char)word[i])) { //check if the character is alphanumeric.
            normalized[j++] = tolower((unsigned char)word[i]); //convert to lowercase and add to normalized word.
        }
    }
    normalized[j] = '\0'; //add a NULL-temrinalization to the normalized string
    return normalized; //return pointer to normalized string
}

//extract snippet around a specified word position
char *extractSnippet(const char *text, int word_position) {
    int start = word_position - 50; //calculate start position for the snippet
    int end = word_position + 50; //calculate end position for the snippet
    if (start < 0) start = 0; //ensure start position is not less than 0
    if (end > (int)strlen(text)) end = strlen(text); //ensure end position doesn't exceed text length
    int actual_length = end - start; //calculate the actual length of the snippet
    if (actual_length <= 0) return NULL; //if length is zero or negative, return NULL
    char *snippet = malloc(actual_length + 1); //allocate memory for the snippet
    if (!snippet) return NULL; //check for malloc failure
    strncpy(snippet, text + start, actual_length); //copy the snippet from the text
    snippet[actual_length] = '\0'; //add NULL-terminate to the snippet string
    return snippet; //return pointer to the snippet
}

//structure to hold a document with their computed relevance score
typedef struct {
    Document *doc; //pointer to document structure
    int relevance; //integer repsresenting the relevance score of the document
} ScoredDocument;

//comparison function to sort ScoredDocument structures by relevance in descending order
int compareByRelevanceDesc(const void *a, const void *b) {
    ScoredDocument *da = (ScoredDocument *)a; //cast void pointer to ScoredDocument pointer
    ScoredDocument *db = (ScoredDocument *)b; //cast void pointer to ScoredDocument pointer
    return db->relevance - da->relevance; //sort in descending order (higher relevance first)
}

//search for documents based on a given query using a reverse index (hash map)
void searchDocuments(HashMap *index, QueryNode *query) {
    if (!index || !query) return; //if index or query is NULL, do nothing
    HashSet *results = NULL; //HashSet to store unique matching documents
    QueryNode *current = query; //pointer to traverse the query linked list

    //iterate through each word in the search query
    while (current) {
        char *normalizedWord = normalizeWord(current->word); //normalize the current query word
        if (!normalizedWord) { //if normalization fails or word is empty, move to the next query word
            current = current->next;
            continue;
        }

        HashSet *docIds = NULL; //HashSet to store document IDs found for the current word
        if (searchHashMap(index, normalizedWord, (void **)&docIds)) { //search for the reverse index for the normalized word
            if (!current->is_excluded) {//if the query word is not marked as excluded
                if (!results) results = createHashSet(); //if results (set of all matching documents) is not yet created, create it
                for (size_t i = 0; i < docIds->size; i++) { //iterate through the coduments found for the current word
                    if (docIds->elements[i]) { //check if element slot is not empty
                        insertHashSet(results, docIds->elements[i]); //insert the matching document into the overall results set.
                    }
                }
            }
        }
        free(normalizedWord); //free memory allocated for the normalized word
        current = current->next; //move to the next word in the query
    }

    if (results && results->size > 0) { //after processing all query words, if any results were found
        ScoredDocument *scored = malloc(results->size * sizeof(ScoredDocument)); //allocate memory for an array of SortedDocument structures
        int count = 0; //counter for the number of documents added to scored array
        for (size_t i = 0; i < results->capacity; i++) { //iterate through the elements of the results HashSet
            if (results->elements[i]) { //check if the element slot is not empty.
                Document *doc = (Document *)results->elements[i]; //cast void pointer to Document pointer
                /*
                int base_relevance = graphGetIndegree(global_graph, doc->id);
                int boost = 0;

                for (QueryNode *q = query; q; q = q->next) {
                    char *norm = normalizeWord(q->word);
                    if (norm) {
                        if (strstr(doc->title, norm)) {
                            boost += 10;  // match in title
                        } else if (strstr(doc->body, norm)) {
                            boost += 1;   // match in body
                        }
                        free(norm);
                    }
                }
                */ //Short and full barrels
                scored[count].doc = doc; //store the document
                scored[count].relevance = graphGetIndegree(global_graph, doc->id); //calculate the relevance using the indegree from the global graph
                count++; //increment the count of scored documents
            }
        }

        qsort(scored, count, sizeof(ScoredDocument), compareByRelevanceDesc); //sort the scored documents array by relevance in descending order

        for (int i = 0; i < count && i < 5; i++) { //print the top 5 relevant documents
            printf("(%d) %s\n", i, scored[i].doc->title); //prnint document rank and title
            printf("---\n");
            char *snippet = extractSnippet(scored[i].doc->body, 10); //extract and print a snippet from the document body, snippet around position 10
            if (snippet) { 
                printf("%s\n", snippet); //print thr snippet
                free(snippet); //free memory allocated for the snippet
            }
            printf("---\nrelevance score: %d\n\n", scored[i].relevance); //print relevance score
        }

        printf("[%d results]\n", count); //print total number of results found
        free(scored); //free memory allocated for the scored documents array
    } else {
        printf("No matching documents found.\n"); //if no matching documents were found print this
    }

    freeHashSet(results); //free the HashSet used to stored initial search results
}

//build reverse index from a list of documents
HashMap *buildReverseIndex(DocumentNode *docs) {
    HashMap *index = createHashMap(1000); //create a new HashMap for the reverse index with an initial capacity of 1000
    if (!index) return NULL; //check for HashMap creation failure

    //traverse the linked list of DocumentNode structures
    while (docs) {
        //duplicate the title and body strings because strtok modifies the string in place
        char *title = strdup(docs->doc->title); 
        char *body  = strdup(docs->doc->body);
        if (!title || !body) { freeHashMap(index); return NULL; } //check for strdup failures, free the partially built HashMap, free any successfully duplicated string

        //process words from the document title
        for (char *tok = strtok(title, " \n\t.,;!?()[]{}"); tok; tok = strtok(NULL, " \n\t.,;!?()[]{}")) { //strtok tokenizes the string using specified delimiters(space, newline, tab, punctuation)
            char *w = normalizeWord(tok); //normalize the current token (word)
            if (w) { //if normalization was successful (word is not empty)
                HashSet *set = NULL; //pointer to a HashSet of documents
                if (searchHashMap(index, w, (void**)&set)) { 
                    insertHashSet(set, docs->doc); //if word exists, add the current documents to its HashSet
                } else {
                    set = createHashSet(); //if word is new, create a new HashSet for it
                    insertHashSet(set, docs->doc); //add the current document to the new HashSet
                    insertHashMap(index, w, set); //insert the new word-HashSet pair into the HashMap
                }
                free(w); //free memory allocated for the normalized word
            }
        }
        //process words from the document body, similar to title processing
        for (char *tok = strtok(body, " \n\t.,;!?()[]{}"); tok; tok = strtok(NULL, " \n\t.,;!?()[]{}")) {
            char *w = normalizeWord(tok);
            if (w) {
                HashSet *set = NULL;
                if (searchHashMap(index, w, (void**)&set)) {
                    insertHashSet(set, docs->doc);
                } else {
                    set = createHashSet();
                    insertHashSet(set, docs->doc);
                    insertHashMap(index, w, set);
                }
                free(w);
            }
        }
        free(title); //free duplicated title string
        free(body); //free duplicated body string
        docs = docs->next; //return the pointer to the built reverse index
    }
    return index;
}

//performs a linear search for documents containing a query word
void searchDocumentsLinear(DocumentNode *docs, QueryNode *query) {
    int printed = 0; //counter for the number of documents printed
    for (DocumentNode *node = docs; node && printed < 5; node = node->next) { //iterate through the linked list of DocumentNodes until end or 5 documents are printed
        Document *d = node->doc; //get document pointer from the current node
        //check if the query word exists in the document body
        if (strcasestr(d->body, query->word)) {
            printf("(%d) %s\n", d->id, d->title); //print document ID and title
            printf("---\n");
            printed++; //increment the count of printed documents
        }
    }
}

void saveReverseIndex(HashMap *index, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return;
    }

    for (size_t i = 0; i < index->capacity; i++) {
        if (index->keys[i] != NULL) {
            fprintf(f, "%s:", index->keys[i]);
            HashSet *set = (HashSet *)index->values[i];
            for (size_t j = 0; j < set->capacity; j++) {
                if (set->elements[j] != NULL) {
                    fprintf(f, "%d,", ((Document*)set->elements[j])->id);
                }
            }
            fprintf(f, "\n");
        }
    }

    fclose(f);
}

HashMap* loadReverseIndex(const char *filename, DocumentNode *docs) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    HashMap *index = createHashMap(10);
    char line[1024];

    while (fgets(line, sizeof(line), f)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;

        *colon = '\0';
        char *word = line;
        char *ids_str = colon + 1;

        HashSet *set = createHashSet();
        char *token = strtok(ids_str, ",");
        while (token != NULL) {
            int id = atoi(token);

            // find document by id
            Document *doc = NULL;
            for (DocumentNode *curr = docs; curr; curr = curr->next) {
                if (curr->doc->id == id) {
                    doc = curr->doc;
                    break;
                }
            }

            if (doc) insertHashSet(set, doc);
            token = strtok(NULL, ",");
        }

        insertHashMap(index, strdup(word), set);
    }

    fclose(f);
    return index;
}
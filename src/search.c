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
            printf("---\nrelevance score: %d\n\n", scored[i].relevance);
        }

        printf("[%d results]\n", count);
        free(scored);
    } else {
        printf("No matching documents found.\n");
    }

    freeHashSet(results);
}

// Build reverse index
HashMap *buildReverseIndex(DocumentNode *docs) {
    HashMap *index = createHashMap(1000);
    if (!index) return NULL;

    while (docs) {
        char *title = strdup(docs->doc->title);
        char *body  = strdup(docs->doc->body);
        if (!title || !body) { freeHashMap(index); return NULL; }

        for (char *tok = strtok(title, " \n\t.,;!?()[]{}"); tok; tok = strtok(NULL, " \n\t.,;!?()[]{}")) {
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
        free(title);
        free(body);
        docs = docs->next;
    }
    return index;
}

// Optional: Linear search fallback
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
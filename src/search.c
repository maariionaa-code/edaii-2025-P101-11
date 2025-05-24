#include "search.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "document.h"
#include "documents_list.h"
#include "graph.h" // NEW: include graph interface

extern DocumentGraph *global_graph; // global graph from main

// Normalize word: lowercase, remove punctuation
char *normalizeWord(const char *word) {
    if (!word) return NULL;
    size_t len = strlen(word);
    char *normalized = malloc(len + 1);
    if (!normalized) return NULL;
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            normalized[j++] = tolower((unsigned char)word[i]);
        }
    }
    normalized[j] = '\0';
    return normalized;
}

// Extract snippet around word position
char *extractSnippet(const char *text, int word_position) {
    int start = word_position - 50;
    int end = word_position + 50;
    if (start < 0) start = 0;
    if (end > (int)strlen(text)) end = strlen(text);
    int actual_length = end - start;
    if (actual_length <= 0) return NULL;
    char *snippet = malloc(actual_length + 1);
    if (!snippet) return NULL;
    strncpy(snippet, text + start, actual_length);
    snippet[actual_length] = '\0';
    return snippet;
}

// Structure to pair documents with their computed relevance
typedef struct {
    Document *doc;
    int relevance;
} ScoredDocument;

int compareByRelevanceDesc(const void *a, const void *b) {
    ScoredDocument *da = (ScoredDocument *)a;
    ScoredDocument *db = (ScoredDocument *)b;
    return db->relevance - da->relevance;
}

// Search with reverse index and show results
void searchDocuments(HashMap *index, QueryNode *query) {
    if (!index || !query) return;

    HashSet *results = NULL;
    QueryNode *current = query;

    while (current) {
        char *normalizedWord = normalizeWord(current->word);
        if (!normalizedWord) {
            current = current->next;
            continue;
        }

        HashSet *docIds = NULL;
        if (searchHashMap(index, normalizedWord, (void **)&docIds)) {
            if (!current->is_excluded) {
                if (!results) results = createHashSet();
                for (size_t i = 0; i < docIds->size; i++) {
                    if (docIds->elements[i]) {
                        insertHashSet(results, docIds->elements[i]);
                    }
                }
            }
        }
        free(normalizedWord);
        current = current->next;
    }

    if (results && results->size > 0) {
        ScoredDocument *scored = malloc(results->size * sizeof(ScoredDocument));
        int count = 0;
        for (size_t i = 0; i < results->capacity; i++) {
            if (results->elements[i]) {
                Document *doc = (Document *)results->elements[i];
                scored[count].doc = doc;
                scored[count].relevance = graphGetIndegree(global_graph, doc->id);
                count++;
            }
        }

        qsort(scored, count, sizeof(ScoredDocument), compareByRelevanceDesc);

        for (int i = 0; i < count && i < 5; i++) {
            printf("(%d) %s\n", i, scored[i].doc->title);
            printf("---\n");
            char *snippet = extractSnippet(scored[i].doc->body, 10);
            if (snippet) {
                printf("%s\n", snippet);
                free(snippet);
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
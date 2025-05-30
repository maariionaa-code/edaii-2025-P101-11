#ifndef SEARCH_H
#define SEARCH_H

#include "query_list.h"
#include "hashmap.h"
#include "documents_list.h"
#include "graph.h"

HashMap *buildReverseIndex(DocumentNode *docs);

void searchDocuments(HashMap *index, QueryNode *query);

void searchDocumentsLinear(DocumentNode *docs, QueryNode *query);

char* normalizeWord(const char *word);
char* extractSnippet(const char *text, int word_position);
int compareByRelevanceDesc(const void *a, const void *b);

void saveReverseIndex(HashMap *index, const char *filename);
HashMap* loadReverseIndex(const char *filename, DocumentNode *docs);

extern DocumentGraph *global_graph;

#endif // SEARCH_H

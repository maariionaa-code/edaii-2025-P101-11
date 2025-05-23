#ifndef SEARCH_H
#define SEARCH_H

#include "query_list.h"
#include "hashmap.h"
#include "documents_list.h"

HashMap *buildReverseIndex(DocumentNode *docs);

void searchDocuments(HashMap *index, QueryNode *query);


void searchDocumentsLinear(DocumentNode *docs, QueryNode *query);

void serializeReverseIndex(HashMap *index, const char *filename);
HashMap *deserializeReverseIndex(const char *filename);

#endif // SEARCH_H

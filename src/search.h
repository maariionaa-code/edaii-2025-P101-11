#ifndef SEARCH_H
#define SEARCH_H

#include "query_list.h"
#include "hashmap.h" // Include hashmap.h

// Performs a search of documents based on a query using the reverse index.
void searchDocuments(HashMap *index, QueryNode *query);

#endif // SEARCH_H
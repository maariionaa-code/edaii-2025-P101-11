#ifndef SEARCH_H
#define SEARCH_H

#include "documents_list.h"
#include "query_list.h"

//performs a linear search of documents based on a query
void searchDocumentsLinear(DocumentNode *docs, QueryNode *query);

#endif

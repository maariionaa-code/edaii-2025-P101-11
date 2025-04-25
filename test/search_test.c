#include "../src/search.h"
#include "../src/documents_list.h"
#include "../src/query_list.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void test_linear_search_matches_doc() {
    runningtest("test_linear_search_matches_doc");
    Document *doc = initDocument(1, "Cat", "Cats are great pets");
    DocumentNode *list = NULL;
    appendDocument(&list, doc);
    QueryNode *query = initQueryFromString("cats");
    searchDocumentsLinear(list, query);
    freeQuery(query);
    freeDocuments(list);
    successtest();
}

void search_test() {
    running("search_test");
    test_linear_search_matches_doc();
    success();
}
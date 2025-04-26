#include "../src/search.h"
#include "../src/documents_list.h"
#include "../src/query_list.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// test that linear search correctlu matches a document with a relevant keyword
void test_linear_search_matches_doc() {
    runningtest("test_linear_search_matches_doc");
    // we create a document with a title and a body
    Document *doc = initDocument(1, "Cat", "Cats are great pets");
    // we create  alinked list to hold the docuemnt
    DocumentNode *list = NULL;
    appendDocument(&list, doc);
    // we create a query with the keyword cats
    QueryNode *query = initQueryFromString("cats");
    // we run the linear search to see if the document is matched
    searchDocumentsLinear(list, query);
    // clean
    freeQuery(query);
    freeDocuments(list);
    successtest();
}

// wrapper to group and execute all search-related tests
void search_test() {
    running("search_test");
    test_linear_search_matches_doc();
    success();
}
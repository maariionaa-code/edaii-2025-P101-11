#include "../src/documents_list.h"
#include "utils.h"
#include <stdlib.h>

// we create a test of a document node
void test_createDocumentNode() {
    runningtest("test_createDocumentNode");
    // we create a simple document
    Document *doc = initDocument(1, "Title", "Body");
    if (!doc) {
        // error when null, as always
        errortest("document is NULL");
        return;
    }
    // w ecreate a node from the document
    DocumentNode *node = createDocumentNode(doc);
    // we validate that the node holds the correct document
    assertEqualsInt(node->doc->id, 1);
    // finally we free the memory
    freeDocuments(node);
    successtest();
}

// appents multiple documents to a list
void test_appendDocument() {
    runningtest("test_appendDocument");
    Document *doc1 = initDocument(1, "Doc1", "Body1");
    Document *doc2 = initDocument(2, "Doc2", "Body2");
    if (!doc1 || !doc2) {
        errortest("One or more documents are NULL");
        return;
    }
    DocumentNode *head = NULL;
    // append first document
    appendDocument(&head, doc1); 
    // append second document
    appendDocument(&head, doc2);
    // we verify the order, first doc1
    assertEqualsInt(head->doc->id, 1);
    // then doc2
    assertEqualsInt(head->next->doc->id, 2);
    // finally we free the list
    freeDocuments(head);
    successtest();
}

// wrapper to froup document list tests
void documents_list_test() {
    running("documents_list_test");
    test_createDocumentNode();
    test_appendDocument();
    success();
}

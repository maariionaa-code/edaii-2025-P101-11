#include "../src/document.h"
#include "utils.h"

// test loading a document from a valid file path
void test_initDocumentFromFile_valid() {
    runningtest("test_initDocumentFromFile_valid");
    // we initialize a document from a test file
    Document *doc = initDocumentFromFile("data/sample.txt", 1);
    if (!doc) {
        // error if the document is null
        errortest("document is NULL");
        return;
    }
    // we check that the document id is as expected
    assertEqualsInt(doc->id, 1);
    // the title should not be null
    if (!doc->title) errortest("title is NULL");
    // body should not be null also
    if (!doc->body) errortest("body is NULL");
    // we use this function to clean the document
    freeDocument(doc);
    successtest();
}

// wrapper to group and run the document-related tests
void document_test() {
    running("document_test");
    test_initDocumentFromFile_valid();
    success();
}


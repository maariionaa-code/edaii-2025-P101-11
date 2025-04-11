#include "../src/document.h"
#include "utils.h"

void test_initDocumentFromFile_valid() {
    runningtest("test_initDocumentFromFile_valid");
    Document *doc = initDocumentFromFile("data/sample.txt", 1);
    if (!doc) {
        errortest("Document is NULL");
        return;
    }
    assertEqualsInt(doc->id, 1);
    if (!doc->title) errortest("Title is NULL");
    if (!doc->body) errortest("Body is NULL");
    freeDocument(doc);
    successtest();
}

void document_test() {
    running("document_test");
    test_initDocumentFromFile_valid();
    success();
}


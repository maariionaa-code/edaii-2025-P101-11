#include "../src/document.h"
#include "utils.h"
#include <cstddef>

void test_initDocumentFromFile_valid() {
  runningtest("test_initDocumentFromFile_valid");
  Document *doc = initDocumentFromFile("data/sample.txt", 1);
  assertEqualsInt(doc->id, 1);
  assert(doc->title != NULL);
  assert(doc->body != NULL);
  freeDocument(doc);
  successtest();
}

void document_test() {
  running("document_test");
  test_initDocumentFromFile_valid();
  success();
}

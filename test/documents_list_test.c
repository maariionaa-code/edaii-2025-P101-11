#include "../src/documents_list.h"
#include "utils.h"
#include <cstddef>

void test_createDocumentNode() {
  runningtest("test_createDocumentNode");
  Document *doc = initDocument(1, "Title", "Body");
  DocumentNode *node = createDocumentNode(doc);
  assertEqualsInt(node->doc->id, 1);
  freeDocuments(node);
  successtest();
}

void test_appendDocument() {
  runningtest("test_appendDocument");
  Document *doc1 = initDocument(1, "Doc1", "Body1");
  Document *doc2 = initDocument(2, "Doc2", "Body2");
  DocumentNode *head = NULL;
  appendDocument(&head, doc1);
  appendDocument(&head, doc2);
  assertEqualsInt(head->doc->id, 1);
  assertEqualsInt(head->next->doc->id, 2);
  freeDocuments(head);
  successtest();
}

void documents_list_test() {
  running("documents_list_test");
  test_createDocumentNode();
  test_appendDocument();
  success();
}

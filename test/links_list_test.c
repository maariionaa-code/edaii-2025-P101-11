#include "../src/links_list.h"
#include "utils.h"
#include <stdlib.h>

void test_createLinkNode() {
    runningtest("test_createLinkNode");
    LinkNode *node = createLinkNode(5);
    if (!node) {
        errortest("Link node is NULL");
        return;
    }
    assertEqualsInt(node->target_id, 5);
    freeLinks(node);
    successtest();
}

void test_appendLink() {
    runningtest("test_appendLink");
    LinkNode *head = NULL;
    appendLink(&head, 1);
    appendLink(&head, 2);
    assertEqualsInt(head->target_id, 1);
    assertEqualsInt(head->next->target_id, 2);
    freeLinks(head);
    successtest();
}

void links_list_test() {
    running("links_list_test");
    test_createLinkNode();
    test_appendLink();
    success();
}

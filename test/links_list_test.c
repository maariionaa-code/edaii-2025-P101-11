#include "../src/links_list.h"
#include "utils.h"
#include <stdlib.h>

// test creation of a link node
void test_createLinkNode() {
    runningtest("test_createLinkNode");
    // we create a link to a document id 5
    LinkNode *node = createLinkNode(5);
    if (!node) {
        errortest("Link node is NULL");
        return;
    }
    // we verify the link destination
    assertEqualsInt(node->target_id, 5);
    // clean 
    freeLinks(node);
    successtest();
}

// test appending links to a link list
void test_appendLink() {
    runningtest("test_appendLink");
    LinkNode *head = NULL;
    // we append the first link
    appendLink(&head, 1);
    // we append the second link
    appendLink(&head, 2);
    assertEqualsInt(head->target_id, 1);
    assertEqualsInt(head->next->target_id, 2);
    // clean
    freeLinks(head);
    successtest();
}

// wrapper to a group link list test
void links_list_test() {
    running("links_list_test");
    test_createLinkNode();
    test_appendLink();
    success();
}

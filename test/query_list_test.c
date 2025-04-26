#include "../src/query_list.h"
#include "utils.h"

// test a basic query with two terms, it ensures that both terms are parsed into nodes correctly
void test_simple_query() {
    runningtest("test_simple_query");
    // we have create a query from string
    QueryNode *q = initQueryFromString("cats dogs");
    // the first word should not be marked as excluded
    assertEqualsInt(q->is_excluded, 0);
    // the word of the first node should be cats
    assertEquals(q->word, "cats");
    // the second node should contain the word dogs
    assertEquals(q->next->word, "dogs");
    // finally we free the memory
    freeQuery(q);
    successtest();
}

// test query with an exclusion term, this checks that exclusion flags are correctly parsed and assigned
void test_query_with_exclusion() {
    runningtest("test_query_with_exclusion");
    // query wuth one exclusion term
    QueryNode *q = initQueryFromString("cats -dogs");

    // first node is cats and should not be excluded
    assertEqualsInt(q->is_excluded, 0);
    assertEquals(q->word, "cats");

    // second node is dogs and should be excluded
    assertEqualsInt(q->next->is_excluded, 1);
    assertEquals(q->next->word, "dogs");
    freeQuery(q); // free memory
    successtest();
}

// test parsing of an empty query string, it shold return null pointer to indicate no query nodes
void test_empty_query() {
    runningtest("test_empty_query");
    // pass empty string
    QueryNode *q = initQueryFromString("");

    // the result should be null because there are no tokens to parse
    assertNull(q);
    successtest();
}

// wrapper to group all query list test for execution
void query_list_test() {
    running("query_list_test");
    test_simple_query();
    test_query_with_exclusion();
    test_empty_query();
    success();
}
#include "../src/query_list.h"
#include "utils.h"

void test_simple_query() {
    runningtest("test_simple_query");
    QueryNode *q = initQueryFromString("cats dogs");
    assertEqualsInt(q->is_excluded, 0);
    assertEquals(q->word, "cats");
    assertEquals(q->next->word, "dogs");
    freeQuery(q);
    successtest();
}

void test_query_with_exclusion() {
    runningtest("test_query_with_exclusion");
    QueryNode *q = initQueryFromString("cats -dogs");
    assertEqualsInt(q->is_excluded, 0);
    assertEquals(q->word, "cats");
    assertEqualsInt(q->next->is_excluded, 1);
    assertEquals(q->next->word, "dogs");
    freeQuery(q);
    successtest();
}

void test_empty_query() {
    runningtest("test_empty_query");
    QueryNode *q = initQueryFromString("");
    assertNull(q);
    successtest();
}

void query_list_test() {
    running("query_list_test");
    test_simple_query();
    test_query_with_exclusion();
    test_empty_query();
    success();
}
// test/hashmap_test.c
#include "../src/hashmap.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

void test_hashmap_basic_insert_search() {
  runningtest("test_hashmap_basic_insert_search");
  HashMap *map = createHashMap(4);
  int a = 42, b = 7;
  insertHashMap(map, "foo", &a);
  insertHashMap(map, "bar", &b);

  void *out = NULL;
  int foundA = searchHashMap(map, "foo", &out);
  assertEqualsInt(foundA, 1);
  assertEqualsInt(*(int*)out, 42);

  int foundB = searchHashMap(map, "bar", &out);
  assertEqualsInt(foundB, 1);
  assertEqualsInt(*(int*)out, 7);

  successtest();
  freeHashMap(map);
}

void test_hashmap_overwrite() {
  runningtest("test_hashmap_overwrite");
  HashMap *map = createHashMap(4);
  int x = 1, y = 2;
  insertHashMap(map, "key", &x);
  insertHashMap(map, "key", &y);

  void *out = NULL;
  int found = searchHashMap(map, "key", &out);
  assertEqualsInt(found, 1);
  assertEqualsInt(*(int*)out, 2);

  successtest();
  freeHashMap(map);
}

void test_hashmap_resize_and_collisions() {
  runningtest("test_hashmap_resize_and_collisions");
  HashMap *map = createHashMap(2);
  #define N 10
  int vals[N];
  char keybuf[16];
  for (int i = 0; i < N; i++) {
    vals[i] = i * 10;
    snprintf(keybuf, sizeof(keybuf), "k%d", i);
    insertHashMap(map, keybuf, &vals[i]);
  }

  for (int i = 0; i < N; i++) {
    snprintf(keybuf, sizeof(keybuf), "k%d", i);
    void *out = NULL;
    assertEqualsInt(searchHashMap(map, keybuf, &out), 1);
    assertEqualsInt(*(int*)out, i * 10);
  }
  successtest();
  freeHashMap(map);
}

void hashmap_test() {
  running("hashmap_test");
  test_hashmap_basic_insert_search();
  test_hashmap_overwrite();
  test_hashmap_resize_and_collisions();
  success();
}
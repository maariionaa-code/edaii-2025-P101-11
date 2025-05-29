#include "../src/hashmap.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

//test basic insertion and lookup functionality of the hashmap
void test_hashmap_basic_insert_search() {
  runningtest("test_hashmap_basic_insert_search");
  HashMap *map = createHashMap(4); //create a small hashmap (capacity 4)
  int a = 42, b = 7; //test values to store
  insertHashMap(map, "foo", &a); //insert data
  insertHashMap(map, "bar", &b);

  void *out = NULL; //test retrieval of first value
  int foundA = searchHashMap(map, "foo", &out);
  assertEqualsInt(foundA, 1); //should find the key
  assertEqualsInt(*(int*)out, 42); //should return correct value

  //test retrieval of second value
  int foundB = searchHashMap(map, "bar", &out);
  assertEqualsInt(foundB, 1); //should find the key
  assertEqualsInt(*(int*)out, 7); //should return correct value

  successtest(); //mark test as successful
  freeHashMap(map); //clean up
}

//test behavior when inserting with duplicate keys
void test_hashmap_overwrite() {
  runningtest("test_hashmap_overwrite");
  HashMap *map = createHashMap(4);
  int x = 1, y = 2; //two values to overwrite behavior
  insertHashMap(map, "key", &x); //insert same key twice
  insertHashMap(map, "key", &y); //should overwrite previous value

  //verify the stored value
  void *out = NULL;
  int found = searchHashMap(map, "key", &out);
  assertEqualsInt(found, 1); //key should exist
  assertEqualsInt(*(int*)out, 2); //shoud contain the second value

  successtest();
  freeHashMap(map);
}

//test hashmap's dynamic resizing and collision handling
void test_hashmap_resize_and_collisions() {
  runningtest("test_hashmap_resize_and_collisions");
  HashMap *map = createHashMap(2); //start with very small capacity to force resizing
  //insert multiple items (more than initial capacity)
  #define N 10 //number of test items
  int vals[N]; //array to store test values
  char keybuf[16]; //buffer for generating keys
  for (int i = 0; i < N; i++) {
    vals[i] = i * 10; //test value (0,10,20....)
    snprintf(keybuf, sizeof(keybuf), "k%d", i); //generate key ("ko", "k1",...)
    insertHashMap(map, keybuf, &vals[i]);
  }

  //verify all items can be retrieved
  for (int i = 0; i < N; i++) {
    snprintf(keybuf, sizeof(keybuf), "k%d", i);
    void *out = NULL;
    assertEqualsInt(searchHashMap(map, keybuf, &out), 1); //key should exist
    assertEqualsInt(*(int*)out, i * 10); //correct value should be returned
  }
  successtest();
  freeHashMap(map);
}

//main test runner funciton that executes all hashmap tests
void hashmap_test() {
  running("hashmap_test"); //start test group
  test_hashmap_basic_insert_search(); //test basic functionality
  test_hashmap_overwrite(); //test overwrite behavior
  test_hashmap_resize_and_collisions(); //test resizing and collisions
  success(); //mark entire test group as successful
}
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

typedef struct HashMap {
    char **keys;       // Array of keys (words)
    void **values;     // Array of values (pointers to HashSet of Document)
    size_t capacity;    // Maximum number of entries
    size_t size;        // Current number of entries
} HashMap;

typedef struct HashSet {
    void **elements;
    size_t size;
    size_t capacity;
} HashSet;

HashSet *createHashSet();

void insertHashSet(HashSet *set, void *element);

void freeHashSet(HashSet *set);

HashMap *createHashMap(size_t capacity);

void insertHashMap(HashMap *map, const char *key, void *value);

int searchHashMap(HashMap *map, const char *key, void **value);

void freeHashMap(HashMap *map);

unsigned long hash(const char *str);

void printHashMap(HashMap *map);


#endif // HASHMAP_H
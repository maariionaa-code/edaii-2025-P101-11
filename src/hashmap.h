#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

// Define a simple hash table structure
typedef struct HashMap {
    char **keys;       // Array of keys (words)
    void **values;     // Array of values (pointers to HashSet of Document)
    size_t capacity;    // Maximum number of entries
    size_t size;        // Current number of entries
} HashMap;

// Define a simple HashSet structure
typedef struct HashSet {
    void **elements;
    size_t size;
    size_t capacity;
} HashSet;

// Create a new hash set
HashSet *createHashSet();

// Insert an element into the hash set
void insertHashSet(HashSet *set, void *element);

// Free a hash set
void freeHashSet(HashSet *set);

// Create a new hash map with a given capacity
HashMap *createHashMap(size_t capacity);

// Insert a key-value pair into the hash map
void insertHashMap(HashMap *map, const char *key, void *value);

// Search for a value by key in the hash map
int searchHashMap(HashMap *map, const char *key, void **value);

// Free the memory used by the hash map
void freeHashMap(HashMap *map);

// Hash function (djb2 algorithm)
unsigned long hash(const char *str);

#endif // HASHMAP_H
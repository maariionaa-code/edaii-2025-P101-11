#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

// this structure represents our hash maps
typedef struct HashMap {
    // array of keys
    char **keys;
    // array of values
    void **values;     
    // total allocated slots in the hashmap
    size_t capacity;    
    // current number of inserted key value pairs
    size_t size;        
} HashMap;

// this structure represents our hash sets
// we use it to store unique elements (in our case are pointers to document)
typedef struct HashSet {
    // array of elements
    void **elements; 
    // current number of elements in the set
    size_t size;   
    // maximum number of elements before resizing
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
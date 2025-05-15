#include "hashmap.h"
#include <stdio.h>

// Hash function (djb2 algorithm)
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

// Create a new hash set
HashSet *createHashSet() {
    HashSet *set = malloc(sizeof(HashSet));
    if (!set) return NULL;
    set->capacity = 10;
    set->size = 0;
    set->elements = malloc(set->capacity * sizeof(void *));
    if (!set->elements)
    {
        free(set);
        return NULL;
    }
    for (size_t i = 0; i < set->capacity; i++)
    {
        set->elements[i] = NULL;
    }

    return set;
}

// Insert an element into the hash set
void insertHashSet(HashSet *set, void *element) {
    if (!set || !element) return;

    if (set->size >= set->capacity) {
        // Resize the hash set (double capacity)
        size_t newCapacity = set->capacity * 2;
        void **newElements = realloc(set->elements, newCapacity * sizeof(void *));
        if (!newElements) {
            return; // Handle allocation failure
        }
        for(size_t i = set->capacity; i < newCapacity; i++){
            newElements[i] = NULL;
        }
        set->elements = newElements;
        set->capacity = newCapacity;
    }

    // Check for duplicates before inserting
    for (size_t i = 0; i < set->size; i++) {
        if (set->elements[i] == element) {
            return; // Element already exists, don't insert again
        }
    }

    set->elements[set->size++] = element;
}

// Free a hash set
void freeHashSet(HashSet *set) {
    if (!set) return;
    free(set->elements);
    free(set);
}

// Create a new hash map with a given capacity
HashMap *createHashMap(size_t capacity) {
    HashMap *map = malloc(sizeof(HashMap));
    if (!map) return NULL;
    map->capacity = capacity;
    map->size = 0;
    map->keys = malloc(capacity * sizeof(char *));
    map->values = malloc(capacity * sizeof(void *));
    if (!map->keys || !map->values) {
        free(map->keys);
        free(map->values);
        free(map);
        return NULL;
    }
     for (size_t i = 0; i < map->capacity; i++) {
        map->keys[i] = NULL;
        map->values[i] = NULL;
    }
    return map;
}

// Insert a key-value pair into the hash map
void insertHashMap(HashMap *map, const char *key, void *value) {
    if (!map || !key) return;
    if (map->size >= map->capacity) {
        // Resize the hash map (double capacity)
        size_t newCapacity = map->capacity * 2;
        char **newKeys = realloc(map->keys, newCapacity * sizeof(char *));
        void **newValues = realloc(map->values, newCapacity * sizeof(void *));
        if (!newKeys || !newValues) {
            return; // Handle allocation failure
        }
        for(size_t i = map->capacity; i < newCapacity; i++){
            newKeys[i] = NULL;
            newValues[i] = NULL;
        }
        map->keys = newKeys;
        map->values = newValues;
        map->capacity = newCapacity;
    }
    unsigned long index = hash(key) % map->capacity;
    // Linear probing to find an empty slot
    while (map->keys[index] != NULL) {
        index = (index + 1) % map->capacity;
    }
    map->keys[index] = strdup(key);
     if (!map->keys[index])
     {
        perror("Strdup failed in insertHashMap");
        return;
     }
    map->values[index] = value;
    map->size++;
}

// Search for a value by key in the hash map
int searchHashMap(HashMap *map, const char *key, void **value) {
    if (!map || !key) return 0;
    unsigned long index = hash(key) % map->capacity;
    // Linear probing to find the key
    while (map->keys[index] != NULL) {
        if (strcmp(map->keys[index], key) == 0) {
            *value = map->values[index];
            return 1; // Key found
        }
        index = (index + 1) % map->capacity;
    }
    return 0; // Key not found
}

// Free the memory used by the hash map
void freeHashMap(HashMap *map) {
    if (!map) return;
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->keys[i] != NULL) {
            free(map->keys[i]);
        }
        if (map->values[i] != NULL) {
            freeHashSet(map->values[i]); // free the hashSet
        }
    }
    free(map->keys);
    free(map->values);
    free(map);
}

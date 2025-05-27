#include "hashmap.h"
#include "document.h"
#include <stdio.h>

// hash function based on the djb2 algorithm by dan bernstein
// it generates a numberic hash from a string key
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// this function is to create and initialize a new empty hash set
HashSet *createHashSet() {
    HashSet *set = malloc(sizeof(HashSet));
    if (!set) return NULL;
    // set initial capacity
    set->capacity = 10;
    set->size = 0;
    set->elements = malloc(set->capacity * sizeof(void *));
    if (!set->elements)
    {
        free(set);
        return NULL;
    }
    // initialize all elements to null
    for (size_t i = 0; i < set->capacity; i++)
    {
        set->elements[i] = NULL;
    }

    return set;
}

// insert an element into the has set created
void insertHashSet(HashSet *set, void *element) {
    if (!set || !element) return;

    // we resize the set if it is full
    if (set->size >= set->capacity) {
        size_t newCapacity = set->capacity * 2;
        void **newElements = realloc(set->elements, newCapacity * sizeof(void *));
        if (!newElements) {
            return; // if realloc fails
        }
        // we initialize new eleemnts to null
        for(size_t i = set->capacity; i < newCapacity; i++){
            newElements[i] = NULL;
        }
        set->elements = newElements;
        set->capacity = newCapacity;
    }

    // finally check if element already exists, duplications are not allowed
    for (size_t i = 0; i < set->size; i++) {
        if (set->elements[i] == element) {
            return; // if lement already exists, don't insert again
        }
    }

    // if not insert the new element
    set->elements[set->size++] = element;
}

// free the memory allocated by a hash set
void freeHashSet(HashSet *set) {
    if (!set) return;
    free(set->elements);
    free(set);
}

//  create a new hash map with a given as a parameter capacity
HashMap *createHashMap(size_t capacity) {
    HashMap *map = malloc(sizeof(HashMap));
    if (!map) return NULL;
    map->capacity = capacity;
    map->size = 0;
    // array of key strings
    map->keys = malloc(capacity * sizeof(char *));
    // array of pointer to values
    map->values = malloc(capacity * sizeof(void *));
    if (!map->keys || !map->values) {
        free(map->keys);
        free(map->values);
        free(map);
        return NULL;
    }
    // initialize all slots to null
    for (size_t i = 0; i < map->capacity; i++) {
        map->keys[i] = NULL;
        map->values[i] = NULL;
    }
    return map;
}

// insert a key-value pair into the hash map using open addressing tih linear probing 
void insertHashMap(HashMap *map, const char *key, void *value) {
    if (!map || !key) return;
    if (map->size >= map->capacity) {
        // resize the hash map if full
        size_t newCapacity = map->capacity * 2;
        char **newKeys = realloc(map->keys, newCapacity * sizeof(char *));
        void **newValues = realloc(map->values, newCapacity * sizeof(void *));
        if (!newKeys || !newValues) {
            return; // Handle allocation failure
        }
        // initialize new slots to null
        for(size_t i = map->capacity; i < newCapacity; i++){
            newKeys[i] = NULL;
            newValues[i] = NULL;
        }
        map->keys = newKeys;
        map->values = newValues;
        map->capacity = newCapacity;
    }
    unsigned long index = hash(key) % map->capacity;
    // find an empty slot
    while (map->keys[index] != NULL) {
        index = (index + 1) % map->capacity;
    }
    // wwe insert the key and the value and make a copy f the key
    map->keys[index] = strdup(key);
     if (!map->keys[index])
     {
        perror("Strdup failed in insertHashMap");
        return;
     }
    map->values[index] = value;
    map->size++;
}

// this function search for a value by key in the hash map
// return 1 if found, 0 otherwise
int searchHashMap(HashMap *map, const char *key, void **value) {
    if (!map || !key) return 0;
    unsigned long index = hash(key) % map->capacity;
    // as before, search using linear probing
    while (map->keys[index] != NULL) {
        if (strcmp(map->keys[index], key) == 0) {
            // we retrueve the value
            *value = map->values[index];
            // key found == 1
            return 1;
        }
        index = (index + 1) % map->capacity;
    }
    // key not found == 0
    return 0;
}

// this function free the memory used by the hash map
void freeHashMap(HashMap *map) {
    if (!map) return;
    // free each key and the hashset value
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->keys[i] != NULL) {
            free(map->keys[i]);
        }
        if (map->values[i] != NULL) {
            // assme all the values are hash sets
            freeHashSet(map->values[i]);
        }
    }
    free(map->keys);
    free(map->values);
    free(map);
}

// this function prints the contents of the hash map, each key and the associated document id
void printHashMap(HashMap *map) {
    if (!map) {
        printf("null hashmap\n");
        return;
    }
    // we iterate over the map and print each key and its associated document ids
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->keys[i]) {
            printf("%s:", map->keys[i]);
            HashSet *set = (HashSet*)map->values[i];
            if (set) {
                for (size_t j = 0; j < set->capacity; j++) {
                    if (set->elements[j]) {
                        Document *d = (Document*)set->elements[j];
                        // print the docuemnt id
                        printf(" %d", d->id);
                    }
                }
            }
            printf("\n");
        }
    }
}


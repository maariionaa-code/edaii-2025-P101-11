#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUERIES 3
static char *recent_queries[MAX_QUERIES];
static int total = 0;

void enqueueQuery(const char *query) {
    if (total == MAX_QUERIES) {
        free(recent_queries[0]);
        for (int i = 1; i < MAX_QUERIES; i++) {
            recent_queries[i - 1] = recent_queries[i];
        }
        total--;
    }
    recent_queries[total++] = strdup(query);
}

void printRecentQueries() {
    printf("\n******* Recent Searches ********\n");
    for (int i = 0; i < total; i++) {
        printf("* %s *\n", recent_queries[i]);
    }
    printf("********************************\n");
}

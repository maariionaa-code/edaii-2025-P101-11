#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUERIES 3 //maximum number of recent queries to store
static char *recent_queries[MAX_QUERIES]; //array to hold the recent query strings
static int total = 0; //current number of queries in the queue

//adds a new query to the queue of recent searches
void enqueueQuery(const char *query) {
    if (total == MAX_QUERIES) { //if the queue is full
        free(recent_queries[0]); //free the oldest query
        for (int i = 1; i < MAX_QUERIES; i++) { //shift remaining queries to the left
            recent_queries[i - 1] = recent_queries[i];
        }
        total--; //decrement the total count
    }
    recent_queries[total++] = strdup(query); //add the new query to the end of the queue and duplicate the query string
}

//prints the list of recent searches
void printRecentQueries() {
    printf("\n******* Recent Searches ********\n"); 
    for (int i = 0; i < total; i++) {
        printf("* %s *\n", recent_queries[i]); //print each recent query
    }
    printf("********************************\n");
}

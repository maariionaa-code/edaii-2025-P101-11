#ifndef QUERY_LIST_H
#define QUERY_LIST_H

//structure definition for a node in the query list
typedef struct QueryNode {
    char *word; //pointer to a dynamically allocated string storing the query word.
    int is_excluded;  //1 if word starts with '-', 0 otherwise, flag that indicates if the word should be excluded from the search
    struct QueryNode *next; //pointer to next node in linked list
} QueryNode;

QueryNode* initQueryFromString(const char *input); //funtion to initialize query list

void freeQuery(QueryNode *head); //function to free memory allocated for a query list

void printQuery(QueryNode *head); //function to print the contents of a query list

#endif // QUERY_LIST_H

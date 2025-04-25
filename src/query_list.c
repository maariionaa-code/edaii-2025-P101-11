#include "query_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//creates a query node, takes a word and an exclusion flag as intput, returns pointer to the new query node created
QueryNode* createQueryNode(const char *word, int is_excluded) {
    QueryNode *node = malloc(sizeof(QueryNode)); //allocate memory for query node structure
    node->word = strdup(word); //duplicate input word to allocate memory for it and copy content
    node->is_excluded = is_excluded; //assign the is_excluded flag to the node
    node->next = NULL; //initialize the next pointer of the new node to NULL
    return node;
}

//initialize a query linked list from an input string
QueryNode* initQueryFromString(const char *input) {
    QueryNode *head = NULL, *tail = NULL; //initialize head and tail of the linked list to null (empty list)

    char *copy = strdup(input); //create a mutable copy of the input string using strdup, as strtok modifies the string.
    char *token = strtok(copy, " "); //tokenize the copied string using space (" ") as the delimiter.
    while (token) {
        int excluded = (token[0] == '-'); //check if the current token starts with -, indication it should be excluded from the search
        if (excluded) token++; //skip the '-'

        QueryNode *node = createQueryNode(token, excluded); //create a new QueryNode for the current token and its exclusion status.
        if (!head) head = node; //if the list is currently empty (head is NULL), the new node becomes the head.
        else tail->next = node; //otherwise, append the new node to the end of the list
        tail = node; //update the tail pointer to the newly added node (the last node)
        token = strtok(NULL, " "); //get the next token from the string
    }

    free(copy); //free memory allocated for the copied input string
    return head; //return the head of the created query linked list
}

//function to free the memory allocated for a query linked list
void freeQuery(QueryNode *head) {
    while (head) { //iterate through linked list until head becomes null (end of list)
        QueryNode *tmp = head; //store the current head node in a temporary pointer
        head = head->next; //move the head pointer to the next node in the list
        free(tmp->word); //free memory allocated for the word in the current node
        free(tmp); //free memory allocated for the current node itself
    }
}

//funciton to print the words in a query linked list
void printQuery(QueryNode *head) {
    while (head) {
        printf("[%s%s] ", head->is_excluded ? "-" : "", head->word);
        head = head->next; //move the head pointer to the next node
    }
    printf("\n");
}

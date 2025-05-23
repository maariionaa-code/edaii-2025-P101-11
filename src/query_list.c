#include "query_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <search.h>

//creates a query node, takes a word and an exclusion flag as intput, returns pointer to the new query node created
QueryNode* createQueryNode(const char *word, int is_excluded) {
    QueryNode *node = malloc(sizeof(QueryNode)); //allocate memory for query node structure
    node->word = strdup(word); //duplicate input word to allocate memory for it and copy content
    node->is_excluded = is_excluded; //assign the is_excluded flag to the node
    node->next = NULL; //initialize the next pointer of the new node to NULL
    return node;
}

QueryNode* initQueryFromString(const char *input) {
    QueryNode *head = NULL, *tail = NULL;
    char *copy = strdup(input);
    char *token = strtok(copy, " ");
    int in_or_group = 0;
    while (token) {
        if (strcmp(token, "(") == 0) {
            in_or_group = 1;
        } else if (strcmp(token, ")") == 0) {
            in_or_group = 0;
        } else if (strcmp(token, "|") == 0) {
        } else {
            int excluded = (token[0] == '-');
            if (excluded) token++;
            char *norm = normalizeWord(token);
            QueryNode *node = malloc(sizeof(QueryNode));
            node->word = norm ? norm : strdup("");
            node->is_excluded = excluded;
            node->is_or_group = in_or_group;
            node->next = NULL;
            if (!head) head = node;
            else tail->next = node;
            tail = node;
        }
        token = strtok(NULL, " ");
    }
    free(copy);
    return head;
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

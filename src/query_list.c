#include "query_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

QueryNode* createQueryNode(const char *word, int is_excluded) {
    QueryNode *node = malloc(sizeof(QueryNode));
    node->word = strdup(word);
    node->is_excluded = is_excluded;
    node->next = NULL;
    return node;
}

QueryNode* initQueryFromString(const char *input) {
    QueryNode *head = NULL, *tail = NULL;

    char *copy = strdup(input);
    char *token = strtok(copy, " ");
    while (token) {
        int excluded = (token[0] == '-');
        if (excluded) token++; // Skip the '-'

        QueryNode *node = createQueryNode(token, excluded);
        if (!head) head = node;
        else tail->next = node;
        tail = node;
        token = strtok(NULL, " ");
    }

    free(copy);
    return head;
}

void freeQuery(QueryNode *head) {
    while (head) {
        QueryNode *tmp = head;
        head = head->next;
        free(tmp->word);
        free(tmp);
    }
}

void printQuery(QueryNode *head) {
    while (head) {
        printf("[%s%s] ", head->is_excluded ? "-" : "", head->word);
        head = head->next;
    }
    printf("\n");
}

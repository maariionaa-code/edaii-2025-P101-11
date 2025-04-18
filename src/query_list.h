#ifndef QUERY_LIST_H
#define QUERY_LIST_H

typedef struct QueryNode {
    char *word;
    int is_excluded;  // 1 if word starts with '-', 0 otherwise
    struct QueryNode *next;
} QueryNode;

QueryNode* initQueryFromString(const char *input);
void freeQuery(QueryNode *head);
void printQuery(QueryNode *head);

#endif // QUERY_LIST_H

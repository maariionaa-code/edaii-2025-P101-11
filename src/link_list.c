#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "links_list.h"

LinkNode* createLinkNode(const char *link) {
    LinkNode *node = malloc(sizeof(LinkNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->link = malloc(strlen(link) + 1);
    if (!node->link) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->link, link);
    node->next = NULL;
    return node;
}

void appendLink(LinkNode **head, const char *link) {
    if (*head == NULL) {
        *head = createLinkNode(link);
        return;
    }
    LinkNode *current = *head;
    while (current->next) {
        current = current->next;
    }
    current->next = createLinkNode(link);
}

void freeLinks(LinkNode *head) {
    LinkNode *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp->link);
        free(tmp);
    }
}

void printLinks(LinkNode *head) {
    int index = 0;
    while (head) {
        printf("[%d] %s\n", index, head->link);
        head = head->next;
        index++;
    }
}
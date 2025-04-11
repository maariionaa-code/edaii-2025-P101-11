#include <stdio.h>
#include <stdlib.h>
#include "links_list.h"

LinkNode* createLinkNode(int target_id) {
    LinkNode *node = malloc(sizeof(LinkNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->target_id = target_id;
    node->next = NULL;
    return node;
}

void appendLink(LinkNode **head, int target_id) {
    if (*head == NULL) {
        *head = createLinkNode(target_id);
        return;
    }
    LinkNode *current = *head;
    while (current->next) {
        current = current->next;
    }
    current->next = createLinkNode(target_id);
}

void freeLinks(LinkNode *head) {
    LinkNode *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void printLinks(LinkNode *head) {
    while (head) {
        printf("-> %d ", head->target_id);
        head = head->next;
    }
    printf("\n");
}

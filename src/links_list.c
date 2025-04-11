#include <stdio.h>
#include <stdlib.h>
#include "links_list.h"

// this function creates a new link node with the ID of destination specified
LinkNode* createLinkNode(int target_id) {
    // we allocate some memory for the new node
    LinkNode *node = malloc(sizeof(LinkNode));

    // if the assignation fails, we print an error and finish the program
    if (!node) {
        fprintf(stderr, "error allocationg memory\n");
        exit(EXIT_FAILURE);
    }

    // we assign the dest id
    node->target_id = target_id;

    // we initialize the pointer to NULL, because the last link has to be pointing to something
    node->next = NULL;

    return node;
}

// this function adds a new node at the final part of the list
void appendLink(LinkNode **head, int target_id) {
    // in the case that the list is empty, the new node is the head node, we do this assignation
    if (*head == NULL) {
        *head = createLinkNode(target_id);
        return;
    }

    // if it is not empty, we need a while to arrive to the last part of the list, because we want to add it in the final
    LinkNode *current = *head;
    while (current->next) {
        current = current->next;
    }

    // we add the node in the last node
    current->next = createLinkNode(target_id);
}

// this function free all the memory associated to the list of link nodes
void freeLinks(LinkNode *head) {

    // we declare a temporal pointer to save the actual node before doing the free
    LinkNode *tmp;

    // we go through the list when there are still some nodes
    while (head) {

        // we save the node in the temporal pointer
        tmp = head;

        //we actualize the pointer to the next node of the list
        head = head->next;

        // finally we free the memory of the node that we saved
        free(tmp);
    }
}

// this function prints the id of destination of all the links of the list
void printLinks(LinkNode *head) {

    // as always, we go through the list
    while (head) {
        // we print the id that ppoints to the actual link
        printf("-> %d ", head->target_id);

        // go to the next node
        head = head->next;
    }

    // tab
    printf("\n");
}

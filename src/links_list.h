#ifndef LINKS_LIST_H
#define LINKS_LIST_H

// we have created a structure of a link node
// each node has an id of the dest document and a pointer signing to the next node
typedef struct LinkNode {
    int target_id;
    struct LinkNode *next;
} LinkNode;

// declaration of functions defined in links_list.c

// this function creates a new link node with the ID of destination specified
LinkNode* createLinkNode(int target_id);

// this function adds a new node at the final part of the list
void appendLink(LinkNode **head, int target_id);

// this function free all the memory associated to the list of link nodes
void freeLinks(LinkNode *head);

// this function prints the id of destination of all the links of the list
void printLinks(LinkNode *head);

#endif // LINKS_LIST_H
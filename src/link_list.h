#ifndef LINKS_LIST_H
#define LINKS_LIST_H

// Node for a link
typedef struct LinkNode {
    char *link;             // e.g., URL or a reference string
    struct LinkNode *next;
} LinkNode;

// Function prototypes for the links linked list
LinkNode* createLinkNode(const char *link);
void appendLink(LinkNode **head, const char *link);
void freeLinks(LinkNode *head);
void printLinks(LinkNode *head);

#endif // LINKS_LIST_H
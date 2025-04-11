#ifndef LINKS_LIST_H
#define LINKS_LIST_H

typedef struct LinkNode {
    int target_id;
    struct LinkNode *next;
} LinkNode;

LinkNode* createLinkNode(int target_id);
void appendLink(LinkNode **head, int target_id);
void freeLinks(LinkNode *head);
void printLinks(LinkNode *head);

#endif // LINKS_LIST_H
#ifndef DOCUMENTS_LIST_H
#define DOCUMENTS_LIST_H

#include "document.h"

typedef struct DocumentNode {
    Document *doc;
    struct DocumentNode *next;
} DocumentNode;

DocumentNode* createDocumentNode(Document *doc);
void appendDocument(DocumentNode **head, Document *doc);
void freeDocuments(DocumentNode *head);
void printDocuments(DocumentNode *head);
DocumentNode* loadDocuments(const char *directoryPath);

#endif // DOCUMENTS_LIST_H
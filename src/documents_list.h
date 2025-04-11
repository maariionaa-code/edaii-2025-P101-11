#ifndef DOCUMENTS_LIST_H
#define DOCUMENTS_LIST_H

#include "document.h" //needed for the Document structure definition

//linked list node that holds a pointer to a Document
typedef struct DocumentNode {
    Document *doc;               //pointer to the document data
    struct DocumentNode *next;   //pointer to the next node in the list
} DocumentNode;

//creates a new DocumentNode for a given Document
DocumentNode* createDocumentNode(Document *doc);

//appends a DocumentNode to the end of the list
void appendDocument(DocumentNode **head, Document *doc);

//frees all memory used by the linked list of documents
void freeDocuments(DocumentNode *head);

//prints all documents' IDs and titles (and links)
void printDocuments(DocumentNode *head);

//loads all documents from a specified folder into a linked list
DocumentNode* loadDocuments(const char *directoryPath);

#endif // DOCUMENTS_LIST_H
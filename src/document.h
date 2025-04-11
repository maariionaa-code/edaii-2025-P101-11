#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "links_list.h"

typedef struct Document {
    int id;
    char *title;
    char *body;
    LinkNode *links;
} Document;

Document* initDocument(int id, const char *title, const char *body);
Document* initDocumentFromFile(const char *filepath, int id);
void freeDocument(Document *doc);
void printDocument(Document *doc);
void parseLinks(Document *doc); 

#endif // DOCUMENT_H
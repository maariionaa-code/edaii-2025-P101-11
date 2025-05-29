#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "links_list.h" //includes the definition for the LinkNode type

//structure representing a Document
typedef struct Document {
    int id;            //unique identifier for the document
    char *title;       //title of the document (first line of the file)
    char *body;        //full body/content of the document
    LinkNode *links;   //linked list of outgoing links ([link:ID] in the body)
    float relevance;   //stores indegree
} Document;

//initializes a document with given id, title, and body
Document* initDocument(int id, const char *title, const char *body);

//loads a document from a file: first line is title, rest is body.
//also parses links after loading.
Document* initDocumentFromFile(const char *filepath, int id);

//frees all memory used by a document, including title, body, and links
void freeDocument(Document *doc);

//prints the document details (ID, title, body, links) to the terminal
void printDocument(Document *doc);

//parses the document body and adds links ([link:ID]) to the links list
void parseLinks(Document *doc); 

Document* documentDesserialize(char* path);

#endif // DOCUMENT_H
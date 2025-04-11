#include "document.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//initializes a Document with id, title, and body.
//duplicates the strings and sets the links to NULL.
Document* initDocument(int id, const char *title, const char *body) {
    Document *doc = malloc(sizeof(Document));
    if (!doc) return NULL;

    doc->id = id;
    doc->title = strdup(title);
    doc->body = strdup(body);
    doc->links = NULL;

    return doc;
}

//parses the document body to find [link:ID] patterns.
//for each match, it appends a link to the document's links list.
void parseLinks(Document *doc) {
    const char *p = doc->body;
    while ((p = strstr(p, "[link:")) != NULL) {
        int id;
        if (sscanf(p, "[link:%d]", &id) == 1) {
            appendLink(&doc->links, id);
        }
        p += 6; // Move forward to avoid infinite loop
    }
}

//loads a document from a file, using the first line as the title
//and the rest as the body. Parses links after loading.
Document* initDocumentFromFile(const char *filepath, int id) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filepath);
        return NULL;
    }

    //read first line as the title
    char title[256];
    if (!fgets(title, sizeof(title), file)) {
        fclose(file);
        return NULL;
    }
    title[strcspn(title, "\n")] = 0; //remove newline

    //read entire file into content (body)
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);

    //initialize the document and parse its links
    Document *doc = initDocument(id, title, content);
    if (doc) parseLinks(doc);

    return doc;
}

//frees memory used by the document, including title, body, and links.
void freeDocument(Document *doc) {
    if (!doc) return;
    free(doc->title);
    free(doc->body);
    freeLinks(doc->links);
    free(doc);
}

//prints document ID, title, body, and all its links.
void printDocument(Document *doc) {
    if (!doc) return;
    printf("ID: %d\nTitle: %s\nBody:\n%s\n", doc->id, doc->title, doc->body);
    printLinks(doc->links);
}

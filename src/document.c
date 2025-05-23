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

    char discard[256];
    if (!fgets(discard, sizeof(discard), file)) {
        fclose(file);
        return NULL;
    }

    char title_buf[256];
    if (!fgets(title_buf, sizeof(title_buf), file)) {
        fclose(file);
        return NULL;
    }
    title_buf[strcspn(title_buf, "\n")] = '\0'; 

    long start = ftell(file);
    fseek(file, 0, SEEK_END);
    long end = ftell(file);
    long body_len = end - start;
    fseek(file, start, SEEK_SET);

    char *body_buf = malloc(body_len + 1);
    if (!body_buf) {
        fclose(file);
        return NULL;
    }
    fread(body_buf, 1, body_len, file);
    body_buf[body_len] = '\0';
    fclose(file);

    Document *doc = initDocument(id, title_buf, body_buf);
    free(body_buf);
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

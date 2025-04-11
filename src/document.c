#include "document.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Document* initDocument(int id, const char *title, const char *body) {
    Document *doc = malloc(sizeof(Document));
    if (!doc) return NULL;

    doc->id = id;
    doc->title = strdup(title);
    doc->body = strdup(body);
    doc->links = NULL; // You could parse links from body here

    return doc;
}

Document* initDocumentFromFile(const char *filepath, int id) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filepath);
        return NULL;
    }

    char title[256];
    if (!fgets(title, sizeof(title), file)) {
        fclose(file);
        return NULL;
    }
    title[strcspn(title, "\n")] = 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);

    return initDocument(id, title, content);
}

void freeDocument(Document *doc) {
    if (!doc) return;
    free(doc->title);
    free(doc->body);
    freeLinks(doc->links);
    free(doc);
}

void printDocument(Document *doc) {
    if (!doc) return;
    printf("ID: %d\nTitle: %s\nBody:\n%s\n", doc->id, doc->title, doc->body);
    printLinks(doc->links);
}

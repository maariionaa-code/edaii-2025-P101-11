#include "document.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to extract file name from a file path.
static char* extractFileName(const char *filepath) {
    const char *p = strrchr(filepath, '/');
    return strdup(p ? p + 1 : filepath);
}

Document* initDocument(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filepath);
        return NULL;
    }
    
    // Allocate and fill in the Document structure
    Document *doc = malloc(sizeof(Document));
    if (!doc) {
        fclose(file);
        return NULL;
    }
    
    doc->filepath = strdup(filepath);
    doc->filename = extractFileName(filepath);
    
    // Get file size for content allocation
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);
    
    doc->content = malloc(filesize + 1);
    if (!doc->content) {
        fclose(file);
        free(doc->filepath);
        free(doc->filename);
        free(doc);
        return NULL;
    }
    
    fread(doc->content, 1, filesize, file);
    doc->content[filesize] = '\0';
    
    fclose(file);
    return doc;
}

void freeDocument(Document *doc) {
    if (doc) {
        free(doc->filepath);
        free(doc->filename);
        free(doc->content);
        free(doc);
    }
}

void printDocument(Document *doc) {
    if (doc) {
        printf("Filename: %s\n", doc->filename);
        printf("Content:\n%s\n", doc->content);
    }
}
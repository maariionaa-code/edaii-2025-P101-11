#ifndef DOCUMENT_H
#define DOCUMENT_H

typedef struct Document {
    char *filepath;
    char *filename;
    char *content;
} Document;

// Initialize a Document from a given file path.
// It allocates memory for filepath, filename, and content.
Document* initDocument(const char *filepath);
void freeDocument(Document *doc);
void printDocument(Document *doc);

//hola laia

#endif // DOCUMENT_H
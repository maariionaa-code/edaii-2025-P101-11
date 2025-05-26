#include "document.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// scan trough the docuemnt body and extract all links IDs of the form [text](id)
// this function appends each found link ID to the doc -> link list
void parseLinks(Document *doc) {
    const char *p = doc->body;
    // we look for [ indicationg the satrt of the link text
    while ((p = strchr(p, '[')) != NULL) {
        // we find closing bracket
        const char *endText = strchr(p, ']');
        // we find the opening parenthesis
        const char *openPar = strchr(endText, '(');
        // we find the closing parenthesis
        const char *closePar = strchr(openPar, ')');
        if (p && endText && openPar && closePar) {
            // buffer to hold numeric ID string
            char numbuf[32];
            // we compute the length of the ID
            int len = closePar - openPar - 1;
            // we only consider numeric ID lengths within buffer
            if (len > 0 && len < 32) {
                // we copy ID substring
                strncpy(numbuf, openPar + 1, len);
                // null terminate
                numbuf[len] = '\0';
                // convert the id string to integer
                int linkId = atoi(numbuf);
                // record th elink in the document's outgoing list
                appendLink(&doc->links, linkId);
            }
            // we want to contiue scanning so the go on advancing the pointer
            p = closePar + 1;
        } else {
            // it breaks if malformed link or if there are not more links left
            break;
        }
    }
}

// this function allocat and initialize a new document struct in memory
// it duplicates the title and body strings
// it starts with empty link list and zero relevance
Document* initDocument(int id, const char *title, const char *body) {
    // we allocate document struct
    Document *doc = malloc(sizeof(Document));
    // we handle allocation failure
    if (!doc) return NULL;
    // we set the document ID
    doc->id = id;
    // we copy the title string
    doc->title = strdup(title);
    // we copy the body string
    doc->body = strdup(body);
    // we initialize empty link list
    doc->links = NULL;
    // default relevance score is 0
    doc->relevance = 0.0f;
    // finally retur the new document pointer
    return doc;
}

// this function deserialize a docuemnt from a file path
Document* documentDesserialize(char *path) {
    // we open the file
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "error: could not open %s\n", path);
        return NULL;
    }

    char line[512];
    // read document ID
    if (!fgets(line, sizeof(line), f)) 
    { 
        // we read the first line which is the id
        fclose(f); return NULL; 
    }
    // we convert the id line to int
    int docId = atoi(line);

    if (!fgets(line, sizeof(line), f)) 
    { 
        // read the title line
        fclose(f); return NULL; 
    }
    // we remove trailing newline
    line[strcspn(line, "\n")] = '\0';
    // we copy the title
    char *title = strdup(line);

    // we calculate the lenght of the body
    fseek(f, 0, SEEK_END);
    long end = ftell(f);
    fseek(f, 0, SEEK_SET);
    // we use this line to skip the id
    fgets(line, sizeof(line), f);
    // and we use this line to skip the title
    fgets(line, sizeof(line), f);
    long start = ftell(f);
    long body_len = end - start;
    
    // we read the body content
    char *body = malloc(body_len + 1);
    fread(body, 1, body_len, f);
    body[body_len] = '\0';
    fclose(f);

    // now we build a document
    Document *doc = malloc(sizeof(Document));
    doc->id = docId;
    doc->title = title;
    doc->body = body;
    doc->links = NULL;
    doc->relevance = 0.0f;

    // we extract links from body text
    parseLinks(doc);
    return doc;
}

// this function is an alternative version of document loading, given file path and id separately
//it reads form the file, ignoring the id line and uses the provided id instead
Document* initDocumentFromFile(const char *filepath, int id) {
    FILE *file = fopen(filepath, "r");
    if (!file) 
    {
        fprintf(stderr, "Could not open file: %s\n", filepath);
        return NULL;
    }

    // we skip the first line (which is the id in the file)
    char discard[256];
    if (!fgets(discard, sizeof(discard), file)) 
    {
        fclose(file);
        return NULL;
    }

    // we read the title line
    char title_buf[256];
    if (!fgets(title_buf, sizeof(title_buf), file))
    {
        fclose(file);
        return NULL;
    }

    // remove newline
    title_buf[strcspn(title_buf, "\n")] = '\0';

    // in this piece of code we ge tthe body starting positing and length
    long start = ftell(file);
    fseek(file, 0, SEEK_END);
    long end = ftell(file);
    long body_len = end - start;
    // rewind to body start
    fseek(file, start, SEEK_SET);

    // read and allocate body content
    char *body_buf = malloc(body_len + 1);
    if (!body_buf) 
    {
        fclose(file);
        return NULL;
    }
    fread(body_buf, 1, body_len, file);
    body_buf[body_len] = '\0';
    fclose(file);

    // as before create the document using our custom oid
    Document *doc = initDocument(id, title_buf, body_buf);
    // free buffer since initdocument does strdup()
    free(body_buf);
    // parse links if document is created successfully
    if (doc) parseLinks(doc);
    return doc;
}

// this function frees all memory associated with a document including the title string,
// the body string, the linked list of links and the document struct itself
void freeDocument(Document *doc) {
    if (!doc) return;
    free(doc->title);
    free(doc->body);
    // provided in link.c:
    freeLinks(doc->links);
    free(doc);
}

// finally this function prints a document's id, title, and full body content to the terminal
void printDocument(Document *doc) {
    if (!doc) return;
    printf("ID: %d\nTitle: %s\nBody:\n%s\n", doc->id, doc->title, doc->body);
    // it calls printlinks to show outgoing links
    printLinks(doc->links);
}
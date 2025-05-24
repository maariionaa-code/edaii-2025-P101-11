#include "document.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void parseLinks(Document *doc) {
    const char *p = doc->body;
    while ((p = strchr(p, '[')) != NULL) {
        const char *endText = strchr(p, ']');
        const char *openPar = strchr(endText, '(');
        const char *closePar = strchr(openPar, ')');
        if (p && endText && openPar && closePar) {
            char numbuf[32];
            int len = closePar - openPar - 1;
            if (len > 0 && len < 32) {
                strncpy(numbuf, openPar + 1, len);
                numbuf[len] = '\0';
                int linkId = atoi(numbuf);
                appendLink(&doc->links, linkId);
            }
            p = closePar + 1;
        } else {
            break;
        }
    }
}

Document* initDocument(int id, const char *title, const char *body) {
    Document *doc = malloc(sizeof(Document));
    if (!doc) return NULL;
    doc->id = id;
    doc->title = strdup(title);
    doc->body = strdup(body);
    doc->links = NULL;
    doc->relevance = 0.0f;
    return doc;
}

Document* documentDesserialize(char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", path);
        return NULL;
    }

    char line[512];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return NULL; }
    int docId = atoi(line);

    if (!fgets(line, sizeof(line), f)) { fclose(f); return NULL; }
    line[strcspn(line, "\n")] = '\0';
    char *title = strdup(line);

    fseek(f, 0, SEEK_END);
    long end = ftell(f);
    fseek(f, 0, SEEK_SET);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    long start = ftell(f);
    long body_len = end - start;
    char *body = malloc(body_len + 1);
    fread(body, 1, body_len, f);
    body[body_len] = '\0';
    fclose(f);

    Document *doc = malloc(sizeof(Document));
    doc->id = docId;
    doc->title = title;
    doc->body = body;
    doc->links = NULL;
    doc->relevance = 0.0f;
    parseLinks(doc);
    return doc;
}

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
#include "documents_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

DocumentNode* createDocumentNode(Document *doc) {
    DocumentNode *node = malloc(sizeof(DocumentNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->doc = doc;
    node->next = NULL;
    return node;
}

void appendDocument(DocumentNode **head, Document *doc) {
    if (*head == NULL) {
        *head = createDocumentNode(doc);
        return;
    }
    DocumentNode *current = *head;
    while (current->next) {
        current = current->next;
    }
    current->next = createDocumentNode(doc);
}

void freeDocuments(DocumentNode *head) {
    DocumentNode *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        freeDocument(tmp->doc);
        free(tmp);
    }
}

void printDocuments(DocumentNode *head) {
    while (head) {
        printf("ID: %d | Title: %s\n", head->doc->id, head->doc->title);
        printLinks(head->doc->links);
        head = head->next;
    }
}

DocumentNode* loadDocuments(const char *directoryPath) {
    DIR *dir = opendir(directoryPath);
    if (!dir) {
        fprintf(stderr, "Could not open directory: %s\n", directoryPath);
        return NULL;
    }

    struct dirent *entry;
    DocumentNode *docList = NULL;
    int id_counter = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, entry->d_name);

        struct stat st;
        if (stat(fullPath, &st) == -1)
            continue;
        if (!S_ISREG(st.st_mode))
            continue;

        Document *doc = initDocumentFromFile(fullPath, id_counter++);
        if (doc) {
            appendDocument(&docList, doc);
        }
    }

    closedir(dir);
    return docList;
}
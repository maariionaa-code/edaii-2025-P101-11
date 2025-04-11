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
    int index = 0;
    while (head) {
        printf("[%d] %s\n", index, head->doc->filename);
        head = head->next;
        index++;
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
    
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        // Build full file path
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, entry->d_name);
        
        // Optionally: check if it's a regular file using stat()
        struct stat st;
        if (stat(fullPath, &st) == -1)
            continue;
        if (!S_ISREG(st.st_mode))
            continue;
        
        Document *doc = initDocument(fullPath);
        if (doc) {
            appendDocument(&docList, doc);
        }
    }
    
    closedir(dir);
    return docList;
}
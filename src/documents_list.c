#include "documents_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

//creates a new DocumentNode to hold a Document in the linked list
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


//appends a new document to the end of the linked list of documents
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

//frees all memory associated with the linked list of documents
void freeDocuments(DocumentNode *head) {
    DocumentNode *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        freeDocument(tmp->doc); //frees title, body, and links
        free(tmp);              //frees the node itself
    }
}

//prints a list of all documents (ID and title) and their links
void printDocuments(DocumentNode *head) {
    while (head) {
        printf("ID: %d | Title: %s\n", head->doc->id, head->doc->title);
        printLinks(head->doc->links); //also print all outgoing links
        head = head->next;
    }
}

//loads all text files from a given directory into a linked list of documents
DocumentNode* loadDocuments(const char *directoryPath) {
    DIR *dir = opendir(directoryPath);
    if (!dir) {
        fprintf(stderr, "Could not open directory: %s\n", directoryPath);
        return NULL;
    }

    struct dirent *entry;
    DocumentNode *docList = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, entry->d_name);

        struct stat st;
        if (stat(fullPath, &st) == -1 || !S_ISREG(st.st_mode))
            continue;

        Document *doc = documentDesserialize(fullPath);
        if (doc) {
            appendDocument(&docList, doc);
        }
    }

    closedir(dir);
    return docList;
}
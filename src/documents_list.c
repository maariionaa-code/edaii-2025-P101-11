#include "documents_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// we need this include to read directory contents
#include <dirent.h>
// we need this one to check file type
#include <sys/stat.h>

//creates a new DocumentNode to hold a Document in the linked list
DocumentNode* createDocumentNode(Document *doc) {
    DocumentNode *node = malloc(sizeof(DocumentNode));
    if (!node) {
        // print error if memory allocaiton fails
        fprintf(stderr, "memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    // we store the pointer to the document
    node->doc = doc;
    // we initialize the next pointer to null
    node->next = NULL;
    // we return the created node
    return node;
}


//appends a new document to the end of the linked list of documents
void appendDocument(DocumentNode **head, Document *doc) {
    if (*head == NULL) {
        // if the list is empty, the new node becomes the head
        *head = createDocumentNode(doc);
        return;
    }
    // if not, we iterate to the end of the list
    DocumentNode *current = *head;
    while (current->next) {
        current = current->next;
    }
    // we appent the new node at the end
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
        printf("id: %d | title: %s\n", head->doc->id, head->doc->title);
        printLinks(head->doc->links); //also print all outgoing links
        head = head->next;
    }
}

//loads all text files from a given directory into a linked list of documents
DocumentNode* loadDocuments(const char *directoryPath) {
    // we first open the directory
    DIR *dir = opendir(directoryPath);
    if (!dir) {
        fprintf(stderr, "could not open directory: %s\n", directoryPath);
        return NULL;
    }

    struct dirent *entry;
    DocumentNode *docList = NULL;

    // we iterate over each file in the directory
    while ((entry = readdir(dir)) != NULL) {
        // we skip special entreis . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        // we build the full path of the file
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, entry->d_name);

        struct stat st;
        // we skip if not a regular file, dfor example we want to skip the directories
        if (stat(fullPath, &st) == -1 || !S_ISREG(st.st_mode))
            continue;
        
        // we use the fucntion createed in document.c to deserialize the document from the file
        Document *doc = documentDesserialize(fullPath);
        if (doc) {
            // add it to the linked list
            appendDocument(&docList, doc);
        }
    }

    // we close the directory
    closedir(dir);
    // then we return the head of the linked list of docuemnts
    return docList;
}
#include <stdio.h>
#include <stdlib.h>
#include "documents_list.h"
#include "document.h"
#include "links_list.h"
#include "sample_lib.h"

int main(int argc, char *argv[]) {
    printf("\nWelcome to Lab 1!\n\n");
    printf("Factorial of 4 is %d\n", fact(4));

    const char *datasetPath = (argc > 1) ? argv[1] : "wikipedia12";

    DocumentNode *docs = loadDocuments(datasetPath);
    if (!docs) {
        fprintf(stderr, "No documents found in %s\n", datasetPath);
        return EXIT_FAILURE;
    }

    printf("\nDocuments in '%s':\n", datasetPath);
    printDocuments(docs);

    printf("\nEnter the index of the document to view: ");
    int index;
    if (scanf("%d", &index) != 1) {
        fprintf(stderr, "Invalid input.\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }

    DocumentNode *current = docs;
    int i = 0;
    while (current && i < index) {
        current = current->next;
        i++;
    }

    if (!current) {
        fprintf(stderr, "Index out of range.\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }

    printf("\nViewing Document ID %d:\n", current->doc->id);
    printDocument(current->doc);

    freeDocuments(docs);

    return EXIT_SUCCESS;
}
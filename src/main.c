#include <stdio.h>
#include <stdlib.h>
#include "documents_list.h"
#include "document.h"
#include "links_list.h"  // if you want to work with links too
#include "sample_lib.h"  // for your fact() function if needed

int main(int argc, char *argv[]) {
    printf("\nWelcome to Lab 1!\n\n");
    printf("Factorial of 4 is %d\n", fact(4));
    
    // Use current directory or a specified one for dataset
    const char *datasetPath = (argc > 1) ? argv[1] : ".";
    
    // Load documents from the dataset
    DocumentNode *docs = loadDocuments(datasetPath);
    if (!docs) {
        fprintf(stderr, "No documents found in %s\n", datasetPath);
        return EXIT_FAILURE;
    }
    
    // Print the documents with their index
    printf("\nDocuments in '%s':\n", datasetPath);
    printDocuments(docs);
    
    // Allow the user to select a document by index
    printf("\nEnter the index of the document to view: ");
    int index;
    if (scanf("%d", &index) != 1) {
        fprintf(stderr, "Invalid input.\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }
    
    // Traverse the documents linked list to get the selected document
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
    
    // Print the selected document's full content
    printf("\nViewing Document: %s\n", current->doc->filename);
    printDocument(current->doc);
    
    // Free the documents list
    freeDocuments(docs);
    
    return EXIT_SUCCESS;
}
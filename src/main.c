#include <stdio.h>
#include <stdlib.h>
#include "documents_list.h"
#include "document.h"
#include "links_list.h"
#include "sample_lib.h"
#include "query_list.h"
#include "search.h"
#include "queue.h"

int main(int argc, char *argv[]) {
    printf("\nWelcome to Lab 1!\n\n");
    printf("Factorial of 4 is %d\n", fact(4));

    const char *datasetPath = (argc > 1) ? argv[1] : "datasets/wikipedia12";

    DocumentNode *docs = loadDocuments(datasetPath);
    if (!docs) {
        fprintf(stderr, "No documents found in %s\n", datasetPath);
        return EXIT_FAILURE;
    }

    printf("\nDocuments in '%s':\n", datasetPath);
    printDocuments(docs);

    while (1) {
        char input[201];
        printRecentQueries();
        printf("Search: ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0'; 
        if (strlen(input) == 0) break;      

        enqueueQuery(input);
        QueryNode *query = initQueryFromString(input);
        searchDocumentsLinear(docs, query);
        freeQuery(query);
    }

    freeDocuments(docs);

    return EXIT_SUCCESS;
}
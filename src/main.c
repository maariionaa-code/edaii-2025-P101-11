#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "documents_list.h"
#include "document.h"
#include "links_list.h"
#include "sample_lib.h"
#include "query_list.h"
#include "search.h"
#include "queue.h"

int main(int argc, char *argv[]) {
    printf("\nWelcome to Lab 3 (Reverse-Index Search)!\n\n");
    printf("Factorial of 4 is %d\n", fact(4));

    const char *datasetPath = (argc > 1) ? argv[1] : "datasets/wikipedia12";

    DocumentNode *docs = loadDocuments(datasetPath);
    if (!docs) {
        fprintf(stderr, "No documents found in %s\n", datasetPath);
        return EXIT_FAILURE;
    }

    printf("\nDocuments in '%s':\n", datasetPath);
    printDocuments(docs);

    HashMap *index = buildReverseIndex(docs);
    if (!index) {
        fprintf(stderr, "Failed to build reverse index\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }

    while (1) {
        char input[201];
        printRecentQueries();
        printf("Search: ");
        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0)
            break;

        enqueueQuery(input);
        QueryNode *query = initQueryFromString(input);

        searchDocuments(index, query);

        freeQuery(query);
    }

    freeHashMap(index);
    freeDocuments(docs);

    return EXIT_SUCCESS;
}

// src/main.c

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
#include "graph.h"
#include "hashmap.h"

int main(int argc, char *argv[]) {
    printf("\nWelcome to Lab 3 (Reverse-Index Search)!\n\n");
    printf("Factorial of 4 is %d\n", fact(4));

    const char *datasetPath = (argc > 1) ? argv[1] : "datasets/wikipedia270";

    // 1) Cargar documentos
    DocumentNode *docs = loadDocuments(datasetPath);
    if (!docs) {
        fprintf(stderr, "No documents found in %s\n", datasetPath);
        return EXIT_FAILURE;
    }
    printf("\nDocuments in '%s':\n", datasetPath);
    printDocuments(docs);

    // 2) Construir índice inverso
    HashMap *index = buildReverseIndex(docs);
    if (!index) {
        fprintf(stderr, "Failed to build reverse index\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }

    // 3) Construir grafo dirigido de documentos y calcular indegree
    //    Asumimos que los IDs de los documentos van de 0 a n-1 en el mismo orden de loadDocuments
    int n = 0;
    for (DocumentNode *tmp = docs; tmp; tmp = tmp->next) {
        n++;
    }

    Graph *g = createGraph(n);
    if (!g) {
        fprintf(stderr, "Failed to create document graph\n");
        freeHashMap(index);
        freeDocuments(docs);
        return EXIT_FAILURE;
    }

    // Por cada documento src, añadir una arista src → dest para cada LinkNode
    for (DocumentNode *cur = docs; cur; cur = cur->next) {
        int src = cur->doc->id;
        for (LinkNode *ln = cur->doc->links; ln; ln = ln->next) {
            int dest = ln->target_id;
            if (dest >= 0 && dest < n) {
                addEdge(g, src, dest);
            }
        }
    }

    // 4) Bucle principal de búsqueda
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
        // Más adelante podrás pasar 'g' a la función de búsqueda
        // para ordenar los resultados por indegree

        freeQuery(query);
    }

    // 5) Liberar todo
    freeGraph(g);
    freeHashMap(index);
    freeDocuments(docs);

    return EXIT_SUCCESS;
}

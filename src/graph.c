#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DocumentGraph *createGraph(int numDocs) {
    DocumentGraph *g = malloc(sizeof(DocumentGraph));
    if (!g) return NULL;

    g->size = numDocs;
    g->matrix = malloc(numDocs * sizeof(int*));
    for (int i = 0; i < numDocs; i++) {
        g->matrix[i] = calloc(numDocs, sizeof(int));
    }
    return g;
}

void buildGraph(DocumentGraph *graph, DocumentNode *docs) {
    while (docs) {
        int from = docs->doc->id;
        LinkNode *link = docs->doc->links;
        while (link) {
            int to = link->target_id;
            if (from < graph->size && to < graph->size && from >= 0 && to >= 0) {
                graph->matrix[from][to] = 1;
            }
            link = link->next;
        }
        docs = docs->next;
    }
}

int graphGetIndegree(DocumentGraph *graph, int docId) {
    int count = 0;
    for (int i = 0; i < graph->size; i++) {
        if (graph->matrix[i][docId]) count++;
    }
    return count;
}

void freeGraph(DocumentGraph *graph) {
    if (!graph) return;
    for (int i = 0; i < graph->size; i++)
        free(graph->matrix[i]);
    free(graph->matrix);
    free(graph);
}
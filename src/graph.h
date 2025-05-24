#ifndef GRAPH_H
#define GRAPH_H

#include "documents_list.h"

typedef struct {
    int **matrix;
    int size;
} DocumentGraph;

DocumentGraph *createGraph(int numDocs);
void buildGraph(DocumentGraph *graph, DocumentNode *docs);
int graphGetIndegree(DocumentGraph *graph, int documentId);
void freeGraph(DocumentGraph *graph);

#endif

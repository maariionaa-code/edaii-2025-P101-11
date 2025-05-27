#ifndef GRAPH_H
#define GRAPH_H

#include "documents_list.h"

// structure for a directe graph of documents
typedef struct {
    // adjacency matrix to represent edges between documents
    // matrix[i][j] == 1 means there exists a link from a document i to a doc j
    int **matrix;
    // total number of documents which are the nodes in the graph
    int size;
} DocumentGraph;

DocumentGraph *createGraph(int numDocs);
void buildGraph(DocumentGraph *graph, DocumentNode *docs);
int graphGetIndegree(DocumentGraph *graph, int documentId);
void freeGraph(DocumentGraph *graph);

#endif

#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

<<<<<<< HEAD
// this function created a new document graph structure with a given number of documents, which are the nodes
DocumentGraph *createGraph(int numDocs) {
    // we allocate memory for the graph
    DocumentGraph *g = malloc(sizeof(DocumentGraph));
    // return NULL if allocation fails
    if (!g) return NULL;

    // we store the number of documents
    g->size = numDocs;
    // we allocate memory for the adjacency matrix (rows)
    g->matrix = malloc(numDocs * sizeof(int*));
    // we initialize wach row of the matrix with zeros using calloc
    for (int i = 0; i < numDocs; i++) {
        // each row represemts outgoing edges from a document
=======
DocumentGraph *createGraph(int numDocs) { //creates a new DocumentGraph structure and allocates memory for its adjacency matrix.
    DocumentGraph *g = malloc(sizeof(DocumentGraph)); //allocate memory for the DocumentGraph structure
    if (!g) return NULL; //check if memory allocation was successful
    g->size = numDocs; //set the size of the graph (number of documents/nodes)
    g->matrix = malloc(numDocs * sizeof(int*)); //allocate memory for the array of row pointers (for the 2D adjacency matrix)
    for (int i = 0; i < numDocs; i++) { 
>>>>>>> ef0611e (comments)
        g->matrix[i] = calloc(numDocs, sizeof(int));
    }
    // finally we return the created graph
    return g;
}

// this function builds the directed graph by analyzing links between documents
void buildGraph(DocumentGraph *graph, DocumentNode *docs) {
    // we iterate through each docuemnt in the linked list
    while (docs) {
        // we get the source document id
        int from = docs->doc->id;
        // and we get the list of outgoing links
        LinkNode *link = docs->doc->links;
        // now we iterate through all links of the current docuemnt
        while (link) {
            // target document id (linked document)
            int to = link->target_id;
            // only update the matrix if both ids are within bounds, so first we check it with an if condition
            if (from < graph->size && to < graph->size && from >= 0 && to >= 0) {
                // we mark an edge from from to to
                graph->matrix[from][to] = 1;
            }
            // we move to the next link
            link = link->next;
        }
        // we mode to the next document
        docs = docs->next;
    }
}

// this function calculates the in degree for a given document if, which is the number of incoming links
int graphGetIndegree(DocumentGraph *graph, int docId) {
    int count = 0;
    // we count how many documents have a link pointing to docID
    for (int i = 0; i < graph->size; i++) {
        // if there's an edge from i to docID
        if (graph->matrix[i][docId]) count++;
    }
    // return the total in degree
    return count;
}

// this function frees all memory used by the graph
void freeGraph(DocumentGraph *graph) {
    if (!graph) return;
    // we iterate and free each row of the adjacency matrix
    for (int i = 0; i < graph->size; i++)
        free(graph->matrix[i]);
    // then we free the matrix and the graph structure
    free(graph->matrix);
    free(graph);
}
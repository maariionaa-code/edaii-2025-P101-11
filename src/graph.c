// src/graph.c
#include <stdlib.h>
#include "graph.h"

Graph *createGraph(int V) {
    Graph *g = malloc(sizeof(Graph));
    if (!g) return NULL;
    g->V = V;
    g->indegree = calloc(V, sizeof(int));
    g->adj = malloc(V * sizeof(int*));
    g->adjSize = calloc(V, sizeof(int));
    g->adjCap = malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        g->adjCap[i] = 4;
        g->adj[i] = malloc(g->adjCap[i] * sizeof(int));
    }
    return g;
}

void addEdge(Graph *g, int src, int dest) {
    // expandir si es necesario
    if (g->adjSize[src] == g->adjCap[src]) {
        g->adjCap[src] *= 2;
        g->adj[src] = realloc(g->adj[src], g->adjCap[src] * sizeof(int));
    }
    g->adj[src][g->adjSize[src]++] = dest;
    g->indegree[dest]++; 
}

int getIndegree(Graph *g, int v) {
    return (v >= 0 && v < g->V) ? g->indegree[v] : -1;
}

void freeGraph(Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->V; i++) free(g->adj[i]);
    free(g->adj);
    free(g->adjSize);
    free(g->adjCap);
    free(g->indegree);
    free(g);
}

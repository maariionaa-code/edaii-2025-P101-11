// src/graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include "document.h"

// Grafo con lista de adyacencia y array de grados de entrada
typedef struct {
    int V;             // número de vértices
    int *indegree;     // indegree[v] = grado de entrada de v
    int **adj;         // adj[v] = array dinámico de destinos desde v
    int *adjSize;      // número de vecinos actuales en adj[v]
    int *adjCap;       // capacidad de adj[v]
} Graph;

// Crear un grafo con V vértices (0..V-1)
Graph *createGraph(int V);

// Añadir arista dirigida src→dest
void addEdge(Graph *g, int src, int dest);

// Obtener grado de entrada de un vértice
int getIndegree(Graph *g, int v);

// Liberar memoria del grafo
void freeGraph(Graph *g);

#endif // GRAPH_H

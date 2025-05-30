#include <stdio.h>
#include <assert.h>
#include "../src/graph.h"

void test_create_and_add_edge(){
    DocumentGraph *g = createGraph(5);
    assert(g != NULL);
    assert(g->size == 5);

    for (int i = 0; i < 5; i++) {
        assert(graphGetIndegree(g, i) == 0);
    }

    addEdge(g, 0, 2);
    addEdge(g, 1, 2);
    
    assert(graphGetIndegree(g, 2) == 2);
    
    assert(graphGetIndegree(g, 0) == 0);
    assert(graphGetIndegree(g, 1) == 0);

    freeGraph(g);
    printf("test_create_and_add_edge passed!\n");
}
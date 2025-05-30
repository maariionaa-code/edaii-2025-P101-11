#include <stdio.h>
#include <assert.h>
#include "../src/graph.h"

void test_create_and_add_edge() {
    DocumentGraph *g = createGraph(5);
    assert(g != NULL);
    assert(g->size == 5);

    // Initially indegree should be 0 for all nodes
    for (int i = 0; i < 5; i++) {
        assert(graphGetIndegree(g, i) == 0);
    }

    // Manually add edges by setting matrix[i][j] = 1
    g->matrix[0][2] = 1;
    g->matrix[1][2] = 1;

    // Now document 2 should have indegree 2
    assert(graphGetIndegree(g, 2) == 2);
    // Others should remain 0
    assert(graphGetIndegree(g, 0) == 0);
    assert(graphGetIndegree(g, 1) == 0);

    freeGraph(g);
    printf("test_create_and_add_edge passed!\n");
}
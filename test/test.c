#include "sample_lib_test.c"
#include "document_test.c"
#include "documents_list_test.c"
#include "links_list_test.c"
#include "graph_test.c"
#include "../src/graph.h"
#include "utils.h"

DocumentGraph *global_graph = NULL;

int main() {
    sample_lib_test();
    document_test();
    documents_list_test();
    links_list_test();
    test_create_and_add_edge();
    allsuccess();
}

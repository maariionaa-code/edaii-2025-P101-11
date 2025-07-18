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
#include "time.h"

DocumentGraph *global_graph = NULL;

int compareDocs(const void *a, const void *b) {
    Document *docA = *(Document **)a;
    Document *docB = *(Document **)b;
    return (int)(docB->relevance - docA->relevance);
}

int main(int argc, char *argv[]) {
    printf("\nWelcome to EDA 2!\n\n");
    printf("Factorial of 4 is %d\n", fact(4));
    const char *datasetPath = (argc > 1) ? argv[1] : "datasets/wikipedia270";

    DocumentNode *docs = loadDocuments(datasetPath);
    if (!docs) {
        fprintf(stderr, "No documents found in %s\n", datasetPath);
        return EXIT_FAILURE;
    }

    printf("\nLoaded documents:\n");
    for (DocumentNode *curr = docs; curr; curr = curr->next) {
        printf("ID: %d | Title: %s\n", curr->doc->id, curr->doc->title);
    }

    HashMap *index = buildReverseIndex(docs);
    if (!index) {
        fprintf(stderr, "Failed to build reverse index\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }

    /*const char *indexPath = "reverse_index.txt";
    HashMap *index = NULL;
    FILE *test = fopen(indexPath, "r");
    if(test){
        fclose(test);
        printf("Loading reverse index from file...\n");
        index = loadReverseIndex(indexPath, docs);
    }else{
        printf("Building reverse index...\n");
        index = buildReverseIndex(docs);
        saveReverseIndex(index, indexPath);
    }
    if(!index){
        fprintf(stderr, "Failed to build or load reverse index\n");
        freeDocuments(docs);
        return EXIT_FAILURE;
    }*/ //Serializing/deserializing reverse index

    int numDocs = 0;
    for (DocumentNode *curr = docs; curr; curr = curr->next) {
        if (curr->doc->id >= numDocs)
            numDocs = curr->doc->id + 1;
    }

    global_graph = createGraph(numDocs);
    buildGraph(global_graph, docs);

    while (1) {
        char input[201];
        printRecentQueries();
        printf("Search: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) break;

        clock_t start = clock();

        enqueueQuery(input);
        QueryNode *query = initQueryFromString(input);

        HashSet *results = createHashSet();
        for (QueryNode *q = query; q; q = q->next) {
            char *w = normalizeWord(q->word);
            if (w) {
                HashSet *bucket = NULL;
                if (searchHashMap(index, w, (void**)&bucket)) {
                    for (size_t i = 0; i < bucket->capacity; i++) {
                        if (bucket->elements[i]) {
                            insertHashSet(results, bucket->elements[i]);
                        }
                    }
                    //for(size_t i = 0, count = 0; i < bucket->capacity && count < bucket->size; i++){
                    //  if(bucket->elements[i]){
                    //      insertHashSet(results, bucket->elements[i]);
                    //      count++;
                    //}} Lower search runtime complexity
                }
                free(w);
            }
        }

        for (size_t i = 0; i < results->capacity; i++) {
            if (results->elements[i]) {
                Document *d = results->elements[i];
                d->relevance = graphGetIndegree(global_graph, d->id);
            }
        }

        Document **array = malloc(results->size * sizeof(Document*));
        int idx = 0;
        for (size_t i = 0; i < results->capacity; i++) {
            if (results->elements[i]) array[idx++] = results->elements[i];
        }
        qsort(array, idx, sizeof(Document*), compareDocs);

        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

        // print 5 best
        int limit = idx < 5 ? idx : 5;
        printf("\n");
        for (int i = 0; i < limit; i++) {
            printf("(%d) %s\n---\n", i, array[i]->title);
            if (array[i]->body && strlen(array[i]->body) > 0) {
                for (int j = 0; array[i]->body[j] != '\0'; j++) {
                    putchar(array[i]->body[j]);
                    if (array[i]->body[j] == '\n') break;
                }
            } else {
                printf("(empty body)\n");
            }
            printf("---\nrelevance score: %.0f\n\n", array[i]->relevance);
        }

        printf("[%d results] | Search completed in %.6f seconds.\n", idx, elapsed);
        printf("[%d results]\n", idx);
        printf("\n------------------------------\n");
        printf("Select document (0-%d): ", limit - 1);
        int choice = -1;
        scanf("%d", &choice);
        getchar();

        if (choice >= 0 && choice < limit) {
            Document *selected = array[choice];
            printf("\nID\n%d\n\nTITLE\n%s\n\nRELEVANCE SCORE\n%.0f\n\nBODY\n%s\n\n------------------------------\n",
                   selected->id, selected->title, selected->relevance, selected->body);
        }

        free(array);
        freeQuery(query);
        freeHashSet(results);
    }

    freeGraph(global_graph);
    freeHashMap(index);
    freeDocuments(docs);
    return EXIT_SUCCESS;
}
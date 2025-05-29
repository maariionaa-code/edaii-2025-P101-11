// test/search_index_test.c
#include "../src/search.h"
#include "../src/documents_list.h"
#include "../src/query_list.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

//Modified version of searchDocuments that returns the result HashSet
static HashSet* searchIndexReturnSet(HashMap *index, QueryNode *query) {
  if (!index || !query) return NULL;
  HashSet *results = NULL; //will store our matching documents
  QueryNode *cur = query; //current query term
  while (cur) {
    //normalize the current search term
    char *w = normalizeWord(cur->word);
    if (w) {
      HashSet *ds = NULL; //documents containing this term
      if (searchHashMap(index, w, (void**)&ds) && !cur->is_excluded) { //look up term in inverted index
        if (!results) results = createHashSet();
        //add all matching documents to our results
        for (size_t i = 0; i < ds->size; i++) {
          if (ds->elements[i]) insertHashSet(results, ds->elements[i]);
        }
      }
      free(w);
    }
    cur = cur->next; //move to next query term
  }
  return results;
}

//tests basic search functionality with a single matching document
void test_search_index_single_hit() {
  runningtest("test_search_index_single_hit");
  //create two test documents
  Document *d1 = initDocument(1, "Hello", "foo bar baz");
  Document *d2 = initDocument(2, "World", "lorem ipsum");
  //create document list
  DocumentNode *docs = NULL;
  appendDocument(&docs, d1);
  appendDocument(&docs, d2);

  //build inverted index from documents
  HashMap *idx = buildReverseIndex(docs);

  //create query for term "bar" (should only match document 1)
  QueryNode *q = initQueryFromString("bar");
  //execute search and get results
  HashSet *res = searchIndexReturnSet(idx, q);
  //verify results
  assertEqualsInt(res->size, 1); //should find exactly 1 document
  Document *found = (Document*)res->elements[0];
  assertEqualsInt(found->id, 1); //should be document with ID 1

  //clean up
  freeHashSet(res);
  freeQuery(q);
  freeHashMap(idx);
  freeDocuments(docs);
  successtest();
}

//main rest runner for search index functionality
void search_index_test() {
  running("search_index_test"); //start test group
  test_search_index_single_hit(); //run single test case
  success(); //mark test group as successful
}
// test/search_index_test.c
#include "../src/search.h"
#include "../src/documents_list.h"
#include "../src/query_list.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// Variante de searchDocuments que devuelve el HashSet de resultados
static HashSet* searchIndexReturnSet(HashMap *index, QueryNode *query) {
  // Copia de la lógica de searchDocuments pero acumulando y devolviendo el set
  if (!index || !query) return NULL;
  HashSet *results = NULL;
  QueryNode *cur = query;
  while (cur) {
    char *w = normalizeWord(cur->word);
    if (w) {
      HashSet *ds = NULL;
      if (searchHashMap(index, w, (void**)&ds) && !cur->is_excluded) {
        if (!results) results = createHashSet();
        for (size_t i = 0; i < ds->size; i++) {
          if (ds->elements[i]) insertHashSet(results, ds->elements[i]);
        }
      }
      free(w);
    }
    cur = cur->next;
  }
  return results;
}

void test_search_index_single_hit() {
  runningtest("test_search_index_single_hit");
  // Crear 2 documentos
  Document *d1 = initDocument(1, "Hello", "foo bar baz");
  Document *d2 = initDocument(2, "World", "lorem ipsum");
  DocumentNode *docs = NULL;
  appendDocument(&docs, d1);
  appendDocument(&docs, d2);

  // Índice invertido
  HashMap *idx = buildReverseIndex(docs);

  // Query que sólo matchee "bar"
  QueryNode *q = initQueryFromString("bar");
  HashSet *res = searchIndexReturnSet(idx, q);
  assertEqualsInt(res->size, 1);
  Document *found = (Document*)res->elements[0];
  assertEqualsInt(found->id, 1);

  // Limpieza
  freeHashSet(res);
  freeQuery(q);
  freeHashMap(idx);
  freeDocuments(docs);
  successtest();
}

void search_index_test() {
  running("search_index_test");
  test_search_index_single_hit();
  success();
}
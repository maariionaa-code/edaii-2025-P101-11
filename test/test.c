#include "sample_lib_test.c"
#include "document_test.c"
#include "documents_list_test.c"
#include "links_list_test.c"
#include "utils.h"

int main() {
  sample_lib_test();
  document_test();
  documents_list_test();
  links_list_test();
  allsuccess();
}

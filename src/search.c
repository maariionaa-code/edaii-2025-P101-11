#include "search.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int containsWord(const char *text, const char *word) {
    char *copy = strdup(text);
    char *token = strtok(copy, " \n\t.,;!?()[]{}");
    while (token) {
        if (strcasecmp(token, word) == 0) {
            free(copy);
            return 1;
        }
        token = strtok(NULL, " \n\t.,;!?()[]{}");
    }
    free(copy);
    return 0;
}

int documentMatchesQuery(Document *doc, QueryNode *query) {
    QueryNode *current = query;
    while (current) {
        int found = containsWord(doc->body, current->word) || containsWord(doc->title, current->word);
        if (current->is_excluded && found) return 0;     
        if (!current->is_excluded && !found) return 0;   
        current = current->next;
    }
    return 1;
}

void searchDocumentsLinear(DocumentNode *docs, QueryNode *query) {
    int count = 0;
    while (docs) {
        if (documentMatchesQuery(docs->doc, query)) {
            printf("(%d) %s\n---\n%s\n---\n\n", count, docs->doc->title, docs->doc->body);
            count++;
            if (count == 5) break;
        }
        docs = docs->next;
    }
    if (count == 0) {
        printf("No matching documents found.\n");
    }
}

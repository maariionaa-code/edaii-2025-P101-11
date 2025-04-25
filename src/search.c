#include "search.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//checks if a given word exists within a text
int containsWord(const char *text, const char *word) {
    char *copy = strdup(text); //creates a modifiable copy of the input text
    char *token = strtok(copy, " \n\t.,;!?()[]{}"); //tokenizes the copy using specified delimiters
    while (token) {
        if (strcasecmp(token, word) == 0) { //compates the current token with the target word
            free(copy); //frees the allocated memory for the copy of the text
            return 1; //return 1 if match is found
        }
        token = strtok(NULL, " \n\t.,;!?()[]{}"); //gets the next token from the text
    }
    free(copy); //free allocated memory for the copy of the text (no match found)
    return 0; //return 0 if word is not found in the text
}

//check if a documents matches all words in a query
int documentMatchesQuery(Document *doc, QueryNode *query) {
    QueryNode *current = query; //start from the beginning of the query list
    while (current) {
        int found = containsWord(doc->body, current->word) || containsWord(doc->title, current->word); //check if the current query word exists in the document's title or body
        if (current->is_excluded && found) return 0; //if the query word is excluded and found in the document, the document does not match
        if (!current->is_excluded && !found) return 0; //if query word is not excluded and not found in document, documents does not match
        current = current->next; //move to the next word in the query
    }
    return 1; //returns 1 if all query words are matched according to the exclusion rules
}

//searches a list of docs linearlt for documents that match the query
void searchDocumentsLinear(DocumentNode *docs, QueryNode *query) {
    int count = 0; //counter for the number of matching documents found
    while (docs) {
        if (documentMatchesQuery(docs->doc, query)) { //check if the current document matches the query
            printf("(%d) %s\n---\n%s\n---\n\n", count, docs->doc->title, docs->doc->body); //print the document
            count++; 
            if (count == 5) break; //stop after finding and printing the first 5 matching documents
        }
        docs = docs->next; //move to the next document in the list
    }
    if (count == 0) { //if no matching documents were found
        printf("No matching documents found.\n");
    }
}

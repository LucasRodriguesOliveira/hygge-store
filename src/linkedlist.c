#include <stdlib.h>

#include "linkedlist.h"

LinkedList* linkedlist_new() {
  LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
  list->first = NULL;

  return list;
}

void linkedlist_free(LinkedList* list) {
  Node* node = list->first;
  while(node != NULL) {
    Node* next = node->next;
    free(node);
    node = next;
  }

  free(list);
}

void linkedlist_add(LinkedList* list, Node* node) {
  node->next = list->first;
  list->first = node;
}

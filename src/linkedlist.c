#include <stdlib.h>

#include "linkedlist.h"

LinkedList* linkedlist_new() {
  LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
  list->first = NULL;
  list->last = NULL;
  list->length = 0;

  return list;
}

void linkedlist_free(LinkedList* list, int freeNodeValue) {
  Node* node = list->first;
  while(node != NULL) {
    Node* next = node->next;

    if (freeNodeValue) {
      node_free(node);
    } else {
      free(node);
    }

    node = next;
  }

  free(list);
}

void linkedlist_add(LinkedList* list, Node* node) {
  if (list->first == NULL) {
    list->first = node;
  }

  if (list->last != NULL) {
    list->last->next = node;
  }

  node->previous = list->last;
  list->last = node;

  list->length++;
}

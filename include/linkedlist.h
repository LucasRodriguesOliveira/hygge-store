#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

typedef struct linkedlist LinkedList;

struct linkedlist {
  Node* first;
};

LinkedList* linkedlist_new();
void linkedlist_free(LinkedList* list);
void linkedlist_add(LinkedList* list, Node* node);

#endif

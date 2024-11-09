#ifndef HASHMAP_H
#define HASHMAP_H

#include "node.h"

typedef struct hashmap HashMap;
typedef int (*fn_hash_compare)(Node*, const void*);

struct hashmap {
  int length;
  int capacity;
  Node** nodeList;
};

HashMap* hashmap_new();
void hashmap_free(HashMap* map);
void hashmap_resize(HashMap* map, int (*hashId)(void*));
Node* hashmap_add(HashMap* map, Node* node, int (*hashId)());
int hashmap_genHash(int id, int max_sz);
Node* hashmap_find(HashMap* map, const void* term, int hash, fn_hash_compare);

#endif

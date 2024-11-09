#include <stdlib.h>

#include "node.h"
#include "hashmap.h"

#define HASHMAP_INITIAL_CAPACITY 5
#define HASHMAP_INCREASE_MULTIPLIER 1.5

int hashmap_genHash(int id, int max_sz) {
  int hash_1 = id % max_sz;
  int hash_2 = abs(max_sz - 2 - id % (max_sz - 2));

  return (hash_1 + hash_2) % max_sz;
}

HashMap* hashmap_new() {
  HashMap* map = (HashMap*) malloc(sizeof(HashMap));
  map->length = 0;
  map->capacity = HASHMAP_INITIAL_CAPACITY;
  map->nodeList = (Node**) malloc(map->capacity * sizeof(Node*));

  for (int i = 0; i < map->capacity; i++) {
    map->nodeList[i] = NULL;
  }

  return map;
}

void hashmap_free(HashMap* map) {
  for (int i = 0; i < map->capacity; i++) {
    Node* current = map->nodeList[i];

    while(current != NULL) {
      Node* next = current->next;
      node_free(current);
      current = next;
    }
  }

  free(map);
}
Node* hashmap_find(
  HashMap* map,
  const void* term,
  int hash,
  fn_hash_compare cmp
) {
  Node* node = map->nodeList[hash];

  while (node != NULL) {
    if (cmp(node, term)) {
      return node;
    }

    node = node->next;
  }

  return NULL;
}

void hashmap_resize(HashMap* map, int (*hashId)(void*)) {
  int prevMax = map->capacity;
  Node** prev = map->nodeList;
  map->length = 0;
  map->capacity *= (int) HASHMAP_INCREASE_MULTIPLIER;
  map->nodeList = (Node**) malloc(map->capacity * sizeof(Node*));

  for (int i = 0; i < map->capacity; i++) {
    map->nodeList[i] = NULL;
  }

  for (int i = 0; i < prevMax; i++) {
    if (prev[i]) {
      hashmap_add(map, prev[i], hashId);
    }
  }

  free(prev);
}

Node* hashmap_add(HashMap* map, Node* node, int (*hashId)()) {
  if (map->length > 0.75 * map->capacity) {
    hashmap_resize(map, hashId);
  }

  int hash = hashmap_genHash(hashId(node->value), map->capacity);

  node->next = map->nodeList[hash];
  map->nodeList[hash] = node;
  map->length++;

  return node;
}

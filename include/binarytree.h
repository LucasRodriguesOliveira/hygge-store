#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "metadata.h"

typedef struct binary_tree BTree;

struct binary_tree {
  void* value;
  BTree* left;
  BTree* right;
};

BTree* btree_add(BTree* root, void* value, int (*cmp)(void* a, void* b));
void* btree_find(BTree* root, void* value, int (*cmp)(void* a, void* b));
BTree* btree_remove(BTree* root, void* value, int (*cmp)(void* a, void* b));
void atualizar(BTree* raiz, void* old_value, void* new_value, int (*cmp)(void* a, void* b));
void** btree_to_array(BTree* tree, int* out_size);
void btree_print(BTree* root, void (*fn_print)(void* value));
int btree_size(BTree* root);
void btree_free(BTree* root);

#endif

#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "binarytree.h"

BTree* btree_new(void* value) {
  BTree* tree = (BTree*)malloc(sizeof(BTree));
  tree->left = NULL;
  tree->right = NULL;
  tree->value = value;

  return tree;
}

BTree* btree_add(BTree* root, void* value, int (*cmp)(void* a, void* b)) {
  if (root == NULL) {
    return btree_new(value);
  }

  int cmpResult = cmp(root->value, value);

  if (cmpResult >= 0) {
    root->left = btree_add(root->left, value, cmp);
  }
  else {
    root->right = btree_add(root->right, value, cmp);
  }

  return root;
}

void* btree_find(BTree* root, void* value, int (*cmp)(void* a, void* b)) {
  if (root == NULL) {
    return NULL;
  }

  int cmpResult = cmp(root->value, value);

  if (cmpResult == 0) {
    return root->value;
  }

  if (cmpResult > 0) {
    return btree_find(root->left, value, cmp);
  }

  return btree_find(root->right, value, cmp);
}

void btree_print(BTree* root, void (*fn_print)(void* value)) {
  if (root == NULL) {
    return;
  }

  btree_print(root->left, fn_print);
  fn_print(root->value);
  btree_print(root->right, fn_print);
}

BTree* encontrarMin(BTree* root) {
  while (root->left != NULL) {
    root = root->left;
  }
  return root;
}

BTree* btree_remove(BTree* root, void* value, int (*cmp)(void* a, void* b)) {
  if (root == NULL) {
    return NULL;
  }

  int cmpResult = cmp(root->value, value);

  if (cmpResult > 0) {
    root->left = btree_remove(root->left, value, cmp);
  }
  else if (cmpResult < 0) {
    root->right = btree_remove(root->right, value, cmp);
  }
  else {
    if (root->left == NULL && root->right == NULL) {
      free(root);
      return NULL;
    }
    else if (root->left == NULL) {
      BTree* temp = root;
      root = root->right;
      free(temp);
    }
    else if (root->right == NULL) {
      BTree* temp = root;
      root = root->left;
      free(temp);
    }
    else {
      BTree* temp = encontrarMin(root->right);
      root->value = temp->value;
      root->right = btree_remove(root->right, temp->value, cmp);
    }
  }

  return root;
}

void btree_free(BTree* root) {
  if (root != NULL) {
    btree_free(root->left);
    btree_free(root->right);
    free(root->value);
    free(root);
  }
}

int btree_size(BTree* root) {
  if (root == NULL) {
    return 0;
  }

  return 1 + btree_size(root->left) + btree_size(root->right);
}

static void fill_array(BTree* root, void** array, int* index) {
  if (root == NULL) {
    return;
  }

  fill_array(root->left, array, index);
  array[*index] = root->value;
  (*index)++;
  fill_array(root->right, array, index);
}

void** btree_to_array(BTree* root, int* out_size) {
  int size = btree_size(root);
  *out_size = size;

  void** array = malloc(size * sizeof(void*));

  int index = 0;
  fill_array(root, array, &index);

  return array;
}

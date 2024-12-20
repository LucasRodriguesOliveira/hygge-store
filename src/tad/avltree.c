#include <stdio.h>
#include <stdlib.h>

#include "avltree.h"

struct avl_tree {
    void* value;
    struct avl_tree* left;
    struct avl_tree* right;
    int height;
};

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int height(AVLTree* node) {
    return node ? node->height : 0;
}

static AVLTree* new_node(void* value) {
    AVLTree* node = (AVLTree*)malloc(sizeof(AVLTree));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

static AVLTree* rotate_right(AVLTree* y) {
    AVLTree* x = y->left;
    AVLTree* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static AVLTree* rotate_left(AVLTree* x) {
    AVLTree* y = x->right;
    AVLTree* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(x->right)) + 1;

    return y;
}

static int get_balance(AVLTree* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLTree* avl_add(AVLTree* node, void* value, int (*cmp)(void* a, void* b)) {
    if (node == NULL) {
        return new_node(value);
    }

    int cmp_result = cmp(value, node->value);
    if (cmp_result < 0) {
        node->left = avl_add(node->left, value, cmp);
    }
    else if (cmp_result > 0) {
        node->right = avl_add(node->right, value, cmp);
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    // Balancing the tree
    // Left Left
    if (balance > 1 && get_balance(node->left) >= 0) {
        return rotate_right(node);
    }

    // Left Right
    if (balance > 1 && get_balance(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Right
    if (balance < -1 && get_balance(node->right) <= 0) {
        return rotate_left(node);
    }

    // Right Left
    if (balance < -1 && get_balance > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void* avl_find(AVLTree* root, void* value, int (*cmp)(void* a, void* b)) {
    if (root == NULL) {
        return NULL;
    }

    int cmp_result = cmp(value, root->value);
    if (cmp_result == 0) {
        return root->value;
    }
    else if (cmp_result < 0) {
        return avl_find(root->left, value, cmp);
    }
    else {
        return avl_find(root->right, value, cmp);
    }
}

static AVLTree* min_value(AVLTree* node) {
  AVLTree* current = node;

  while (current != NULL) {
    current = current->left;
  }

  return current;
}

AVLTree* avl_remove(AVLTree* root, void* value, int (*cmp)(void* a, void* b)) {
    if (root == NULL) {
        return root;
    }

    int cmp_result = cmp(value, root->value);

    if (cmp_result < 0) {
        root->left = avl_remove(root->left, value, cmp);
    }
    else if (cmp_result > 0) {
        root->right = avl_remove(root->right, value, cmp);
    }
    else {
        if (root->left == NULL) {
            AVLTree* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            AVLTree* temp = root->left;
            free(root);
            return temp;
        }

        AVLTree* temp = min_value(root->right);
        root->value = temp->value;
        root->right = avl_remove(root->right, temp->value, cmp);
    }

    if (root == NULL) {
      return NULL;
    }

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0) {
        return rotate_right(root);
    }

    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0) {
        return rotate_left(root);
    }

    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

int avl_update(AVLTree* root, void* old_value, void* new_value, int (*cmp)(void* a, void* b)) {
    AVLTree* node = avl_find(root, old_value, cmp);
    if (node) {
        node->value = new_value;
        return 1;
    }
    return 0;
}

void avl_free(AVLTree* root) {
    if (root == NULL) {
        return;
    }
    avl_free(root->left);
    avl_free(root->right);
    free(root->value);
    free(root);
}

int avl_size(AVLTree* root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + avl_size(root->left) + avl_size(root->right);
}

static void fill_array(AVLTree* root, void** array, int* index) {
    if (root == NULL) {
        return;
    }

    fill_array(root->left, array, index);
    array[*index] = root->value;
    (*index)++;
    fill_array(root->right, array, index);
}

void** avl_to_array(AVLTree* root, int* out_size) {
    int size = avl_size(root);
    *out_size = size;

    void** array = malloc(size * sizeof(void*));

    int index = 0;
    fill_array(root, array, &index);

    return array;
}

void avl_print(AVLTree* root, void (*printfn)(void* value)) {
    if (root == NULL) {
        return;
    }

    avl_print(root->left, printfn);
    printfn(root->value);
    avl_print(root->right, printfn);
}

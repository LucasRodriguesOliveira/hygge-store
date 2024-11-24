#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct avl_tree AVLTree;

AVLTree* avl_add(AVLTree* root, void* value, int (*cmp)(void* a, void* b));
void* avl_find(AVLTree* root, void* value, int (*cmp)(void* a, void* b));
AVLTree* avl_remove(AVLTree* root, void* value, int (*cmp)(void* a, void* b));
int avl_update(AVLTree* root, void* old_value, void* new_value, int (*cmp)(void* a, void* b));

#endif

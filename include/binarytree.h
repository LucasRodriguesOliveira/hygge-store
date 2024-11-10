#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "metadata.h"

typedef struct binary_tree BTree;

struct binary_tree {
    void* value;
    BTree* esq;
    BTree* dir;
};

BTree* inserir(BTree* raiz, void* value, int (*cmp)(void* a, void* b));
void buscar(BTree* raiz, void* value, int (*cmp)(void* a, void* b));
BTree* remover(BTree* raiz, void* value, int (*cmp)(void* a, void* b));
void atualizar(BTree* raiz, void* old_value, void* new_value, int (*cmp)(void* a, void* b));
void liberar(BTree* raiz);

#endif
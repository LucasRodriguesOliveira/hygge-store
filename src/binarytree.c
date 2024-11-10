#include <stdlib.h>
#include <string.h>
#include "binarytree.h"

BTree* inserir(BTree* raiz, void* value, int (*cmp)(void* a, void* b)) {
    if (raiz == NULL) {
        raiz = (BTree*) malloc(sizeof(BTree));
        raiz->value = value;
        raiz->esq = raiz->dir = NULL;
    }else if (cmp(value, raiz->value) < 0) {
        raiz->esq = inserir(raiz->esq, value, cmp);
    }else if (cmp(value, raiz->value) > 0) {
        raiz->dir = inserir(raiz->dir, value, cmp);
    }
    return raiz;
}

void* buscar(BTree* raiz, void* value, int (*cmp)(void* a, void* b)) {
    if (raiz == NULL) {
        return NULL;
    }
    int cmp_result = cmp(value, raiz->value);

    if (cmp_result == 0) {
        return raiz->value;
    }else if (cmp_result < 0) {
        return buscar(raiz->esq, value, cmp);
    } else {
        return buscar(raiz-> dir, value, cmp);
    }
}

BTree* encontrarMin(BTree* raiz) {
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}

BTree* remover(BTree* raiz, void* value, int (*cmp)(void* a, void* b)) {
    if (raiz == NULL) {
        return NULL;
    }
    int cmp_result = cmp(value, raiz->value);
    if (cmp_result< 0) {
        raiz->esq = remover(raiz->esq, value, cmp);
    } else if (cmp_result > 0) {
        raiz->dir = remover(raiz->dir, value, cmp);
    } else {
        if (raiz->esq == NULL) {
            BTree* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            BTree* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        BTree* temp = encontrarMin(raiz->dir);
        raiz->value = temp->value;
        raiz->dir = remover(raiz->dir, temp->value, cmp);
    }
    return raiz;
}

void atualizar(BTree* raiz, void* old_value, void* new_value, int (*cmp)(void* a, void* b)) {
    BTree* node = buscar(raiz, old_value, cmp);
    if (node != NULL) {
        node->value = new_value;
    }
}

void liberar(BTree* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esq);
        liberar(raiz->dir);
        free(raiz);
    }
}
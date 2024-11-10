#include <stdlib.h>
#include <string.h>
#include "binarytree.h"

void arvBin_free(ArvBinNode* raiz) {
    if (raiz != NULL) {
        arvBin_free(raiz->esq);
        arvBin_free(raiz->dir);
        free(raiz->metadata);
        free(raiz);
    }
}

ArvBinNode* inserir(ArvBinNode* raiz, Metadata* metadata) {
    if (raiz == NULL) {
        raiz = (ArvBinNode*) malloc(sizeof(ArvBinNode));
        raiz->metadata = metadata;
        raiz->esq = raiz->dir = NULL;
    }else if (strcmp(metadata->filename, raiz->metadata->filename) < 0) {
        raiz->esq = inserir(raiz->esq, metadata);
    }else if (strcmp(metadata->filename, raiz->metadata->filename) > 0) {
        raiz->dir = inserir(raiz->dir, metadata);
    }
    return raiz;
}

ArvBinNode* buscar(ArvBinNode* raiz, const char* filename) {
    if (raiz == NULL || strcmp(filename, raiz->metadata->filename) == 0) {
        return raiz;
    }
    if (strcmp(filename, raiz->metadata->filename) < 0) {
        return buscar(raiz->esq, filename);
    }else {
        return buscar(raiz->dir, filename);
    }
}

ArvBinNode* encontrarMin(ArvBinNode* raiz) {
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}

ArvBinNode* remover(ArvBinNode* raiz, const char* filename) {
    if (raiz == NULL) {
        return raiz;
    }
    if (strcmp(filename, raiz->metadata->filename) < 0) {
        raiz->esq = remover(raiz->esq, filename);
    } else if (strcmp(filename, raiz->metadata->filename) > 0) {
        raiz->dir = remover(raiz->dir, filename);
    }else {
        if (raiz->esq == NULL) {
            ArvBinNode* temp = raiz->dir;
            free(raiz);
            return temp;
        }else if (raiz->dir == NULL) {
            ArvBinNode* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        ArvBinNode* temp = encontrarMin(raiz->dir);
        raiz->metadata = temp->metadata;
        raiz->dir = remover(raiz->dir, temp->metadata->filename);
    }
    return raiz;
}

void atualizar(ArvBinNode* raiz, Metadata* new_metadata) {
    ArvBinNode* node = buscar(raiz, new_metadata->filename);
    if (node != NULL) {
        node->metadata = new_metadata;
    }
}

void liberar(ArvBinNode* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esq);
        liberar(raiz->dir);
        free(raiz);
    }
}
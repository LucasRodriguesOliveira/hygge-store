#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "metadata.h"

typedef struct arvBin_node ArvBinNode;

struct arvBin_node {
    Metadata* metadata;
    ArvBinNode* esq;
    ArvBinNode* dir;
};

ArvBinNode* inserir(ArvBinNode* raiz, Metadata* metadata);
ArvBinNode* buscar(ArvBinNode* raiz, const char* filename);
ArvBinNode* remover(ArvBinNode* raiz, const char* filename);
void atualizar(ArvBinNode* raiz, Metadata* new_metadata);
void arvBin_free(ArvBinNode* raiz);

#endif
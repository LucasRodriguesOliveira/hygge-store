#ifndef PRODUCT_H
#define PRODUCT_H

#include "metadata.h"
#include "binarytree.h"

typedef struct product Product;

struct product {
  int id;
  char name[100];
};

Product* product_new(int id, char* name);
void product_save(Product* p, Metadata* metadata);
Product** product_load(Metadata* metadata);
Product* productTree_findById(BTree* productTree, int id);
void product_freeList(Product** productList, int length);
BTree* product_loadAsBTree(Metadata* metadata);
Product* product_removeById(Metadata* metadata, BTree* productTree, int id);
Product* product_updateById(Metadata* metadata, BTree* productTree, int id, char* name);

#endif

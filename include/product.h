#ifndef PRODUCT_H
#define PRODUCT_H

#include "metadata.h"

typedef struct product Product;

struct product {
  int id;
  char name[100];
};

Product* product_new(int id, char* name);
void product_save(Product* p, Metadata* metadata);
Product** product_load(Metadata* metadata);
Product* product_findById(Metadata* metada, int id);
void product_freeList(Product** productList, int length);

#endif

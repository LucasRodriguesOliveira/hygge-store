#ifndef PRODUCT_H
#define PRODUCT_H

#include "metadata.h"

typedef struct product Product;

struct product {
  int id;
  char name[100];
};

void product_save(Product* p, Metadata* metadata);
Product** product_load(Metadata* metadata);
void product_freeList(Product** productList, int length);
Product* product_new(int id, char* name);

#endif

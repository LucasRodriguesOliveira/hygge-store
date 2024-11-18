#ifndef PRODUCT_MODEL_H
#define PRODUCT_MODEL_H

#include "product.h"

typedef struct product_model ProductModel;

struct product_model {
  Product* (*new)(char* name);
  Product* (*create)(int id, char* name);
  void (*toString)(Product* product);
  void (*freeList)(Product** list, int length);
  void (*columnsToString)();
  void (*printList)(Product** list, int length);
  int (*compare)(void* a, void* b);
  Product* (*copy)(Product* src);
};

ProductModel* productModel_new();

#endif

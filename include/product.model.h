#ifndef PRODUCT_MODEL_H
#define PRODUCT_MODEL_H

#include "category.h"
#include "product.h"

typedef struct product_model ProductModel;

struct product_model {
  Product* (*new)(char* name, double price, int quantity, int categoryId);
  Product* (*create)(int id, char* name, double price, int quantity, int categoryId);
  void (*toString)(Product* product, Category* category);
  void (*freeList)(Product** list, int length);
  void (*printList)(Product** list, int length, Category* (*findCategory)(int id));
  int (*compare)(void* a, void* b);
  Product* (*copy)(Product* src);
};

ProductModel* productModel_new();

#endif

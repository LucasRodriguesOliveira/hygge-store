#ifndef PRODUCT_H
#define PRODUCT_H

#include "metadata.h"
#include "avltree.h"

#define PRODUCT_NAME_LENGTH 100

typedef struct product Product;

struct product {
  int id;
  char name[100];
};

#endif

#ifndef PRODUCT_H
#define PRODUCT_H

#define PRODUCT_NAME_LENGTH 100

typedef struct product Product;

struct product {
  int id;
  char name[100];
  double price;
  int quantity;
  int categoryId;
};

#endif

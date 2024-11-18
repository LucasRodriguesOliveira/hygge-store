#ifndef PRODUCT_REPOSITORY_H
#define PRODUCT_REPOSITORY_H

#include "metadata.h"
#include "product.h"
#include "product.model.h"

typedef struct product_repository ProductRepository;

struct product_repository {
  Metadata* metadata;
  ProductModel* model;

  Product* (*save)(Product* product);
  Product** (*findAll)(void);
  Product* (*findById)(int id);
  Product* (*update)(Product* product);
  Product* (*remove)(int id);
  int (*count)(void);
};

ProductRepository* productRepository_new(Metadata* metadata);

#endif

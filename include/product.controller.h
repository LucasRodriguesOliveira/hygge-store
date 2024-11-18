#ifndef PRODUCT_CONTROLLER_H
#define PRODUCT_CONTROLLER_H

#include "config.h"
#include "metadata.h"
#include "product.repository.h"
#include "product.h"

typedef struct product_controller ProductController;

struct product_controller {
  Metadata* metadata;
  ProductRepository* repository;

  int (*count)(ProductController* self);
  Product** (*list)(ProductController* self);
  Product* (*create)(ProductController* self, char* name);
  Product* (*find)(ProductController* self, int id);
  Product* (*update)(ProductController* self, Product* product);
  Product* (*destroy)(ProductController* self, int id);
};

ProductController* productController_new(Config* config);

#endif

#ifndef CATEGORY_CONTROLLER_H
#define CATEGORY_CONTROLLER_H

#include "config.h"
#include "linkedlist.h"
#include "metadata.h"
#include "product.repository.h"
#include "category.repository.h"
#include "category.h"

typedef struct category_controller CategoryController;

struct category_controller {
  Metadata* metadata;
  CategoryRepository* repository;
  ProductRepository* productRepository;

  int (*count)(CategoryController* self);
  Category** (*list)(CategoryController* self);
  LinkedList* (*listProductsByCategory)(CategoryController* self, int id);
  Category* (*create)(CategoryController* self, char* description);
  Category* (*find)(CategoryController* self, int id);
  Category* (*update)(CategoryController* self, Category* category);
  Category* (*destroy)(CategoryController* self, int id);
};

CategoryController* categoryController_new(Config* config);

#endif

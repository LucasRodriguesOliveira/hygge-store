#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "product.model.h"
#include "product.controller.h"

#define ENTITY "product"
#define CATEGORY_ENTITY "category"

static ProductController* instance = NULL;

static Product* create(
  ProductController* self,
  char* name,
  double price,
  int quantity,
  int categoryId
) {
  ProductModel* model = productModel_new();
  return self->repository->save(model->new(name, price, quantity, categoryId));
}

static Product** list(ProductController* self) {
  return self->repository->findAll();
}

static Product* find(ProductController* self, int id) {
  return self->repository->findById(id);
}

static Product* update(ProductController* self, Product* product) {
  return self->repository->update(product);
}

static Product* destroy(ProductController* self, int id) {
  return self->repository->remove(id);
}

static int count(ProductController* self) {
  return self->repository->count();
}

ProductController* productController_new(Config* config) {
  if (instance != NULL) {
    return instance;
  }

  ProductController* controller =
    (ProductController*)malloc(sizeof(ProductController));

  Metadata* productEntity = config_getMetadata(config, ENTITY);
  Metadata* categoryEntity = config_getMetadata(config, CATEGORY_ENTITY);

  if (productEntity == NULL || categoryEntity == NULL) {
    fprintf(stderr, "\n[Erro]: Não foi possível encontrar os dados.\n");
    exit(EXIT_FAILURE);
  }

  controller->repository = productRepository_new(productEntity, categoryEntity);
  controller->create = create;
  controller->list = list;
  controller->find = find;
  controller->update = update;
  controller->destroy = destroy;
  controller->count = count;

  return controller;
}

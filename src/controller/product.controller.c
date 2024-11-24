#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "avltree.h"
#include "product.model.h"
#include "product.controller.h"

#define ENTITY "product"

static ProductController* instance = NULL;

static Product* create(ProductController* self, char* name) {
  ProductModel* model = productModel_new();
  return self->repository->save(model->new(name));
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

  Metadata* metadata = config_getMetadata(config, ENTITY);
  if (metadata == NULL) {
    fprintf(stderr, "\n[Erro]: Não foi possível encontrar os dados.\n");
    exit(EXIT_FAILURE);
  }

  controller->repository = productRepository_new(metadata);
  controller->create = create;
  controller->list = list;
  controller->find = find;
  controller->update = update;
  controller->destroy = destroy;
  controller->count = count;

  return controller;
}

#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "linkedlist.h"
#include "product.h"
#include "product.controller.h"
#include "category.model.h"
#include "category.controller.h"

#define ENTITY "category"
#define PRODUCT_ENTITY "product"

static CategoryController* instance = NULL;

static Category* create(CategoryController* self, char* description) {
  CategoryModel* model = categoryModel_new();
  return self->repository->save(model->new(description));
}

static Category** list(CategoryController* self) {
  return self->repository->findAll();
}

static LinkedList* listProductsByCategory(CategoryController* self, int id) {
  return self->productRepository->findByCategory(id);
}

static Category* find(CategoryController* self, int id) {
  return self->repository->findById(id);
}

static Category* update(CategoryController* self, Category* category) {
  return self->repository->update(category);
}

static Category* destroy(CategoryController* self, int id) {
  return self->repository->remove(id);
}

static int count(CategoryController* self) {
  return self->repository->count();
}

CategoryController* categoryController_new(Config* config) {
  if (instance != NULL) {
    return instance;
  }

  CategoryController* controller =
    (CategoryController*)malloc(sizeof(CategoryController));

  Metadata* categoryEntity = config_getMetadata(config, ENTITY);
  Metadata* productEntity = config_getMetadata(config, PRODUCT_ENTITY);

  if (categoryEntity == NULL || productEntity == NULL) {
    fprintf(stderr, "\n[Erro]: Não foi possível encontrar os dados.\n");
    exit(EXIT_FAILURE);
  }

  controller->repository = categoryRepository_new(categoryEntity);
  controller->productRepository = productRepository_new(
    productEntity,
    categoryEntity
  );
  controller->create = create;
  controller->list = list;
  controller->listProductsByCategory = listProductsByCategory;
  controller->find = find;
  controller->update = update;
  controller->destroy = destroy;
  controller->count = count;

  return controller;
}

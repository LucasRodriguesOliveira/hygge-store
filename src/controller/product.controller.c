#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "binarytree.h"
#include "product.controller.h"

#define ENTITY "product"

static Product* create(ProductController* self, char* name) {
  int id = self->metadata->nextId++;
  Product* product = product_new(id, name);
  product_save(product, self->metadata);

  return product;
}

static Product** list(ProductController* self) {
  return product_load(self->metadata);
}

static int compareById(void* a, void* b) {
  int id_a = ((Product*)a)->id;
  int id_b = ((Product*)b)->id;

  return id_a - id_b;
}

static Product* find(ProductController* self, int id) {
  BTree* productTree = product_asBTree(self->metadata);

  Product* found = (Product*)btree_find(
    productTree,
    product_new(id, ""),
    compareById
  );

  if (found == NULL) {
    btree_free(productTree);
    return NULL;
  }

  Product* product = product_copy(found);
  btree_free(productTree);

  return product;
}

static Product* update(ProductController* self, Product* product) {
  BTree* productTree = product_asBTree(self->metadata);

  Product* updated = product_updateById(
    self->metadata,
    productTree,
    product->id,
    product->name
  );

  if (updated == NULL) {
    btree_free(productTree);
    return NULL;
  }

  product = product_copy(updated);
  btree_free(productTree);

  return product;
}

static Product* destroy(ProductController* self, int id) {
  BTree* productTree = product_asBTree(self->metadata);

  Product* removed = product_removeById(self->metadata, productTree, id);

  if (removed == NULL) {
    btree_free(productTree);
    return NULL;
  }

  Product* product = product_copy(removed);
  btree_free(productTree);

  return product;
}

static int count(ProductController* self) {
  return self->metadata->count;
}

ProductController* productController_new(Config* config) {
  ProductController* controller =
    (ProductController*)malloc(sizeof(ProductController));

  Metadata* metadata = config_getMetadata(config, ENTITY);
  if (metadata == NULL) {
    fprintf(stderr, "[Erro]: Não foi possível encontrar os dados.\n");
    exit(EXIT_FAILURE);
  }

  controller->metadata = metadata;
  controller->create = create;
  controller->list = list;
  controller->find = find;
  controller->update = update;
  controller->destroy = destroy;
  controller->count = count;

  return controller;
}

#include <stdlib.h>

#include "binarytree.h"
#include "metadata.h"
#include "product.repository.h"
#include "product.model.h"
#include "product.h"

static ProductRepository* instance = NULL;

static void saveData(Product* product) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(product, sizeof(Product), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

static Product* save(Product* product) {
  product->id = instance->metadata->nextId++;
  saveData(product);
  return product;
}

static void override(Product** productList) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_WRITE
  );

  int length = metadata->count;

  for (int i = 0; i < length; i++) {
    fwrite(productList[i], sizeof(Product), 1, file);
  }

  fclose(file);
}

static Product** findAll() {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  Product** products = (Product**)malloc(length * sizeof(Product*));

  for (int i = 0; i < length; i++) {
    products[i] = (Product*)malloc(sizeof(Product));
    fread(products[i], sizeof(Product), 1, file);
  }

  fclose(file);

  return products;
}

static BTree* listAsBTree() {
  Metadata* metadata = instance->metadata;
  ProductModel* model = instance->model;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  BTree* productTree = NULL;

  for (int i = 0; i < length; i++) {
    Product* product = (Product*)malloc(sizeof(Product));

    fread(product, sizeof(Product), 1, file);

    productTree = btree_add(
      productTree,
      product,
      model->compare
    );
  }

  fclose(file);
  return productTree;
}

static Product* findById(int id) {
  ProductModel* model = instance->model;
  BTree* productTree = listAsBTree();

  Product* found = (Product*)btree_find(
    productTree,
    model->create(id, ""),
    model->compare
  );

  Product* product = NULL;

  if (found != NULL) {
    product = model->copy(found);
  }

  btree_free(productTree);
  return product;
}

static Product* destroy(int id) {
  ProductModel* model = instance->model;
  Metadata* metadata = instance->metadata;
  BTree* productTree = listAsBTree();
  Product* productToFind = model->create(id, "");

  Product* removed = (Product*)btree_find(
    productTree,
    productToFind,
    model->compare
  );

  if (removed == NULL) {
    btree_free(productTree);
    free(productToFind);
    return NULL;
  }

  productTree = btree_remove(
    productTree,
    productToFind,
    model->compare
  );

  free(productToFind);

  int productListLength;
  Product** productList = (Product**)btree_to_array(
    productTree,
    &productListLength
  );

  metadata->count = productListLength;

  override(productList);
  metadata_save(metadata);

  Product* product = model->copy(removed);
  btree_free(productTree);
  free(removed);

  return product;
}

static int count(void) {
  return instance->metadata->count;
}

static Product* update(Product* product) {
  Product* updated = destroy(product->id);

  if (updated == NULL) {
    return NULL;
  }

  snprintf(updated->name, 100, "%s", product->name);
  saveData(updated);

  free(product);
  product = instance->model->copy(updated);

  return product;
}

ProductRepository* productRepository_new(Metadata* metadata) {
  if (instance != NULL) {
    return instance;
  }

  instance = (ProductRepository*) malloc(sizeof(ProductRepository));
  instance->model = productModel_new();
  instance->metadata = metadata;
  instance->save = save;
  instance->findAll = findAll;
  instance->findById = findById;
  instance->remove = destroy;
  instance->update = update;
  instance->count = count;

  return instance;
}

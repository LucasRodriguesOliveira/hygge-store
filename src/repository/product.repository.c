#include <stdlib.h>

#include "avltree.h"
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

static AVLTree* listAsAVLTree() {
  Metadata* metadata = instance->metadata;
  ProductModel* model = instance->model;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  AVLTree* productTree = NULL;

  for (int i = 0; i < length; i++) {
    Product* product = (Product*)malloc(sizeof(Product));

    fread(product, sizeof(Product), 1, file);

    productTree = avl_add(
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
  AVLTree* productTree = listAsAVLTree();

  Product* found = (Product*)avl_find(
    productTree,
    model->create(id, ""),
    model->compare
  );

  Product* product = NULL;

  if (found != NULL) {
    product = model->copy(found);
  }

  avl_free(productTree);
  return product;
}

static Product* destroy(int id) {
  ProductModel* model = instance->model;
  Metadata* metadata = instance->metadata;
  AVLTree* productTree = listAsAVLTree();
  Product* productToFind = model->create(id, "");

  Product* removed = (Product*)avl_find(
    productTree,
    productToFind,
    model->compare
  );

  if (removed == NULL) {
    avl_free(productTree);
    free(productToFind);
    return NULL;
  }

  productTree = avl_remove(
    productTree,
    productToFind,
    model->compare
  );

  free(productToFind);

  int productListLength = metadata->count;
  Product** productList = (Product**)malloc(productListLength * sizeof(Product*));
  int index = 0;

  avl_to_array(productTree, (void**)productList, &index);

  metadata->count = productListLength;

  override(productList);
  metadata_save(metadata);

  Product* product = model->copy(removed);
  avl_free(productTree);
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

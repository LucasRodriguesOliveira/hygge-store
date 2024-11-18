#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "product.h"
#include "metadata.h"
#include "binarytree.h"

int compareById(void* a, void* b) {
  int id_a = ((Product*)a)->id;
  int id_b = ((Product*)b)->id;

  return id_a - id_b;
}

Product* product_new(int id, char* name) {
  Product* p = (Product*)malloc(sizeof(Product));
  p->id = id;
  strncpy(p->name, name, 100);

  return p;
}

void product_save(Product* p, Metadata* metadata) {
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(p, sizeof(Product), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

Product** product_load(Metadata* metadata) {
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

void product_override(Metadata* metadata, Product** productList) {
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

BTree* product_asBTree(Metadata* metadata) {
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
    productTree = btree_add(productTree, product, compareById);
  }

  fclose(file);
  return productTree;
}

Product* productTree_findById(BTree* productTree, int id) {
  return (Product*)btree_find(
    productTree,
    product_new(id, ""),
    compareById
  );
}

void product_print(void* ptr) {
  Product* product = (Product*) ptr;

  printf("Product [%d]: %s\n", product->id, product->name);
}

Product* product_removeById(Metadata* metadata, BTree* productTree, int id) {
  Product* productToFind = product_new(id, "");
  Product* removed = (Product*)btree_find(productTree, productToFind, compareById);
  productTree = btree_remove(productTree, productToFind, compareById);

  int productListLength;
  Product** productList = (Product**)btree_to_array(productTree, &productListLength);
  metadata->count = productListLength;

  product_override(metadata, productList);
  metadata_save(metadata);

  return removed;
}

Product* product_updateById(Metadata* metadata, BTree* productTree, int id, char* name) {
  Product* p = product_removeById(metadata, productTree, id);
  snprintf(p->name, 100, "%s", name);

  product_save(p, metadata);
  metadata_save(metadata);

  return p;
}

void product_freeList(Product** productList, int length) {
  for (int i = 0; i < length; i++) {
    free(productList[i]);
  }

  free(productList);
}

Product* product_copy(Product* src) {
  return product_new(src->id, src->name);
}

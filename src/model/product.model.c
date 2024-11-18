#include <stdlib.h>
#include <stdio.h>

#include "product.h"
#include "product.model.h"

static ProductModel* instance = NULL;

static Product* create(int id, char* name) {
  Product* product = (Product*)malloc(sizeof(Product));
  product->id = id;
  snprintf(product->name, PRODUCT_NAME_LENGTH, "%s", name);

  return product;
}

static Product* new(char* name){
  return create(0, name);
}

static void toString(Product* product) {
  printf("| %d\t| %s\n", product->id, product->name);
}

static void columnsToString() {
  printf("\n| Id\t| Nome");
  printf("\n-------------------------\n");
}

static void freeList(Product** list, int length) {
  for (int i = 0; i < length; i++) {
    free(list[i]);
  }

  free(list);
}

static int compare(void* a, void* b) {
  int id_a = ((Product*)a)->id;
  int id_b = ((Product*)b)->id;

  return id_a - id_b;
}

static Product* copy(Product* src) {
  return create(src->id, src->name);
}

static void printList(Product** list, int length) {
  columnsToString();

  for (int i = 0; i < length; i++) {
    toString(list[i]);
  }
}

ProductModel* productModel_new() {
  if (instance != NULL) {
    return instance;
  }

  instance = (ProductModel*) malloc(sizeof(ProductModel));

  instance->create = create;
  instance->new = new;
  instance->copy = copy;
  instance->compare = compare;
  instance->freeList = freeList;
  instance->toString = toString;
  instance->columnsToString = columnsToString;
  instance->printList = printList;

  return instance;
}

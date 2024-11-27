#include <stdlib.h>
#include <stdio.h>

#include "product.h"
#include "category.h"
#include "product.model.h"

static ProductModel* instance = NULL;

static Product* create(
  int id,
  char* name,
  double price,
  int quantity,
  int categoryId
) {
  Product* product = (Product*)malloc(sizeof(Product));
  product->id = id;
  product->price = price;
  product->quantity = quantity;
  product->categoryId = categoryId;
  snprintf(product->name, PRODUCT_NAME_LENGTH, "%s", name);

  return product;
}

static Product* new(char* name, double price, int quantity, int categoryId){
  return create(0, name, price, quantity, categoryId);
}

static void toString(Product* product, Category* category) {
  printf("[%d]: %s\n", product->id, product->name);
  printf("Categoria [%d]: %s\n", category->id, category->description);
  printf("Preço: R$ %8.2lf\n", product->price);
  printf("Quantidade: %d\n", product->quantity);
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
  return create(
    src->id,
    src->name,
    src->price,
    src->quantity,
    src->categoryId
  );
}

static void printList(
  Product** list,
  int length,
  Category* (*findCategory)(int id)
) {
  for (int i = 0; i < length; i++) {
    Product* product = list[i];
    Category* category = findCategory(product->categoryId);

    toString(product, category);
    free(category);
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
  instance->printList = printList;

  return instance;
}

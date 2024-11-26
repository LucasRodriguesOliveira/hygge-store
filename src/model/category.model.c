#include <stdlib.h>
#include <stdio.h>

#include "category.h"
#include "category.model.h"

static CategoryModel* instance = NULL;

static Category* create(int id, char* description) {
  Category* category = (Category*)malloc(sizeof(Category));
  category->id = id;
  snprintf(
    category->description,
    CATEGORY_DESCRIPTION_LENGTH,
    "%s",
    description
  );

  return category;
}

static Category* new(char* description){
  return create(0, description);
}

static void toString(Category* category) {
  printf(
    "| %d\t| %s\n",
    category->id,
    category->description
  );
}

static void columnsToString() {
  printf("\n| Id\t| Descrição");
  printf("\n-----------------------------------------------\n");
}

static void freeList(Category** list, int length) {
  for (int i = 0; i < length; i++) {
    free(list[i]);
  }

  free(list);
}

static int compare(void* a, void* b) {
  int id_a = ((Category*)a)->id;
  int id_b = ((Category*)b)->id;

  return id_a - id_b;
}

static Category* copy(Category* src) {
  return create(src->id, src->description);
}

static void printList(Category** list, int length) {
  columnsToString();

  for (int i = 0; i < length; i++) {
    toString(list[i]);
  }
}

CategoryModel* categoryModel_new() {
  if (instance != NULL) {
    return instance;
  }

  instance = (CategoryModel*)malloc(sizeof(CategoryModel));

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

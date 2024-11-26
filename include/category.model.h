#ifndef CATEGORY_MODEL_H
#define CATEGORY_MODEL_H

#include "category.h"

typedef struct category_model CategoryModel;

struct category_model {
  Category* (*new)(char* description);
  Category* (*create)(int id, char* description);
  void (*toString)(Category* category);
  void (*freeList)(Category** list, int length);
  void (*columnsToString)();
  void (*printList)(Category** list, int length);
  int (*compare)(void* a, void* b);
  Category* (*copy)(Category* src);
};

CategoryModel* categoryModel_new();

#endif

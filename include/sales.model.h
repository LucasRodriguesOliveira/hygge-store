#ifndef SALES_MODEL_H
#define SALES_MODEL_H

#include "sales.h"

typedef struct sales_model SalesModel;

struct sales_model {
  Sales* (*new)(char* name);
  Sales* (*create)(int id, char* name);
  void (*toString)(Sales* sales);
  void (*freeList)(Sales** list, int length);
  void (*columnsToString)();
  void (*printList)(Sales** list, int length);
  int (*compare)(void* a, void* b);
  Sales* (*copy)(Sales* src);
};

SalesModel* salesModel_new();

#endif

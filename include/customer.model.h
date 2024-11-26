#ifndef CUSTOMER_MODEL_H
#define CUSTOMER_MODEL_H

#include "customer.h"

typedef struct customer_model CustomerModel;

struct customer_model {
  Customer* (*new)(char* name);
  Customer* (*create)(int id, char* name);
  void (*toString)(Customer* customer);
  void (*freeList)(Customer** list, int length);
  void (*columnsToString)();
  void (*printList)(Customer** list, int length);
  int (*compare)(void* a, void* b);
  Customer* (*copy)(Customer* src);
};

CustomerModel* customerModel_new();

#endif

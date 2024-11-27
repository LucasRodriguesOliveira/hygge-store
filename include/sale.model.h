#ifndef SALES_MODEL_H
#define SALES_MODEL_H

#include "customer.h"
#include "sale.h"

typedef struct sale_model SaleModel;

struct sale_model {
  Sale* (*new)(int custumerId, double total);
  Sale* (*create)(int id, int custumerId, double total);
  void (*toString)(Sale* sale, Customer* customer);
  void (*freeList)(Sale** list, int length);
  void (*printList)(Sale** list, int length, Customer* (*findCustomer)(int id));
  int (*compare)(void* a, void* b);
  Sale* (*copy)(Sale* src);
};

SaleModel* saleModel_new();

#endif

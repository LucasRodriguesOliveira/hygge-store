#include <stdlib.h>
#include <stdio.h>

#include "customer.h"
#include "sale.h"
#include "sale.model.h"

static SaleModel* instance = NULL;

static Sale* create(int id, int custumerId, double total) {
  Sale* sale = (Sale*)malloc(sizeof(Sale));
  sale->id = id;
  sale->customerId = custumerId;
  sale->total = total;

  return sale;
}

static Sale* new(int custumerId, double total){
  return create(0, custumerId, total);
}

static void toString(Sale* sale, Customer* customer) {
  printf("Venda [%d]:\n", sale->id);
  printf("\tCliente [%d]: %s\n", customer->id, customer->name);
  printf("\tTotal: R$ %8.2lf\n", sale->total);
}

static void freeList(Sale** list, int length) {
  for (int i = 0; i < length; i++) {
    free(list[i]);
  }

  free(list);
}

static int compare(void* a, void* b) {
  int id_a = ((Sale*)a)->id;
  int id_b = ((Sale*)b)->id;

  return id_a - id_b;
}

static Sale* copy(Sale* src) {
  return create(src->id, src->customerId, src->total);
}

static void printList(Sale** list, int length, Customer* (*findCustomer)(int id)) {
  for (int i = 0; i < length; i++) {
    Sale* sale = list[i];
    Customer* customer = findCustomer(sale->customerId);
    toString(sale, customer);
    printf("--------------------------------\n");
    free(customer);
  }
}

SaleModel* saleModel_new() {
  if (instance != NULL) {
    return instance;
  }

  instance = (SaleModel*) malloc(sizeof(SaleModel));

  instance->create = create;
  instance->new = new;
  instance->copy = copy;
  instance->compare = compare;
  instance->freeList = freeList;
  instance->toString = toString;
  instance->printList = printList;

  return instance;
}

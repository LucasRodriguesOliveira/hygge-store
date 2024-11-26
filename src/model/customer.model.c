#include <stdlib.h>
#include <stdio.h>

#include "customer.h"
#include "customer.model.h"

static CustomerModel* instance = NULL;

static Customer* create(int id, char* name) {
  Customer* customer = (Customer*)malloc(sizeof(Customer));
  customer->id = id;
  snprintf(customer->name, CUSTOMER_NAME_LENGTH, "%s", name);

  return customer;
}

static Customer* new(char* name){
  return create(0, name);
}

static void toString(Customer* customer) {
  printf("| %d\t| %s\n", customer->id, customer->name);
}

static void columnsToString() {
  printf("\n| Id\t| Nome");
  printf("\n-------------------------\n");
}

static void freeList(Customer** list, int length) {
  for (int i = 0; i < length; i++) {
    free(list[i]);
  }

  free(list);
}

static int compare(void* a, void* b) {
  int id_a = ((Customer*)a)->id;
  int id_b = ((Customer*)b)->id;

  return id_a - id_b;
}

static Customer* copy(Customer* src) {
  return create(src->id, src->name);
}

static void printList(Customer** list, int length) {
  columnsToString();

  for (int i = 0; i < length; i++) {
    toString(list[i]);
  }
}

CustomerModel* customerModel_new() {
  if (instance != NULL) {
    return instance;
  }

  instance = (CustomerModel*) malloc(sizeof(CustomerModel));

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

#ifndef CUSTOMER_REPOSITORY_H
#define CUSTOMER_REPOSITORY_H

#include "metadata.h"
#include "customer.h"
#include "customer.model.h"

typedef struct customer_repository CustomerRepository;

struct customer_repository {
  Metadata* metadata;
  CustomerModel* model;

  Customer* (*save)(Customer* customer);
  Customer** (*findAll)(void);
  Customer* (*findById)(int id);
  Customer* (*update)(Customer* customer);
  Customer* (*remove)(int id);
  int (*count)(void);
};

CustomerRepository* customerRepository_new(Metadata* metadata);

#endif

#ifndef CUSTOMER_CONTROLLER_H
#define CUSTOMER_CONTROLLER_H

#include "config.h"
#include "metadata.h"
#include "customer.repository.h"
#include "customer.h"

typedef struct customer_controller CustomerController;

struct customer_controller {
  Metadata* metadata;
  CustomerRepository* repository;

  int (*count)(CustomerController* self);
  Customer** (*list)(CustomerController* self);
  Customer* (*create)(CustomerController* self, char* name);
  Customer* (*find)(CustomerController* self, int id);
  Customer* (*update)(CustomerController* self, Customer* customer);
  Customer* (*destroy)(CustomerController* self, int id);
};

CustomerController* customerController_new(Config* config);

#endif

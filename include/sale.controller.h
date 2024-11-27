#ifndef SALES_CONTROLLER_H
#define SALES_CONTROLLER_H

#include "config.h"
#include "linkedlist.h"
#include "metadata.h"
#include "sale.repository.h"
#include "sale.h"

typedef struct sale_controller SaleController;

struct sale_controller {
  Metadata* metadata;
  SaleRepository* repository;

  int (*count)(SaleController* self);
  Sale** (*list)(SaleController* self);
  Sale* (*create)(SaleController* self, int customerId, LinkedList* products);
  Sale* (*find)(SaleController* self, int id);
  Sale* (*update)(SaleController* self, Sale* sale);
  Sale* (*destroy)(SaleController* self, int id);
};

SaleController* saleController_new(Config* config);

#endif

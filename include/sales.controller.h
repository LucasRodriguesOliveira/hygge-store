#ifndef SALES_CONTROLLER_H
#define SALES_CONTROLLER_H

#include "config.h"
#include "metadata.h"
#include "sales.repository.h"
#include "sales.h"

typedef struct sales_controller SalesController;

struct sales_controller {
  Metadata* metadata;
  SalesRepository* repository;

  int (*count)(SalesController* self);
  Sales** (*list)(SalesController* self);
  Sales* (*create)(SalesController* self, char* name);
  Sales* (*find)(SalesController* self, int id);
  Sales* (*update)(SalesController* self, Sales* sales);
  Sales* (*destroy)(SalesController* self, int id);
};

SalesController* salesController_new(Config* config);

#endif

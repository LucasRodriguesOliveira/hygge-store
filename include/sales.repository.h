#ifndef SALES_REPOSITORY_H
#define SALES_REPOSITORY_H

#include "metadata.h"
#include "sales.h"
#include "sales.model.h"

typedef struct sales_repository SalesRepository;

struct sales_repository {
  Metadata* metadata;
  SalesModel* model;

  Sales* (*save)(Sales* sales);
  Sales** (*findAll)(void);
  Sales* (*findById)(int id);
  Sales* (*update)(Sales* sales);
  Sales* (*remove)(int id);
  int (*count)(void);
};

SalesRepository* salesRepository_new(Metadata* metadata);

#endif

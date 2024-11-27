#ifndef SALES_REPOSITORY_H
#define SALES_REPOSITORY_H

#include "metadata.h"
#include "sale.h"
#include "sale.model.h"

typedef struct sale_repository SaleRepository;

struct sale_repository {
  Metadata* metadata;
  SaleModel* model;

  Sale* (*save)(Sale* sale);
  Sale** (*findAll)(void);
  Sale* (*findById)(int id);
  Sale* (*update)(Sale* sale);
  Sale* (*remove)(int id);
  int (*count)(void);
};

SaleRepository* saleRepository_new(Metadata* metadata);

#endif

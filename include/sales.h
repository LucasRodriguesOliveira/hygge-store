#ifndef SALES_H
#define SALES_H

#include "metadata.h"
#include "product.h"
#include "user.h"
#include "config.h"

typedef struct {
    int id;
    int userId;
    int* productIds;
    int productCount;
    float totalValue;
} Sale;

typedef struct SalesController {
    void* repository;
    Sale* (*create)(struct SalesController* self, User* user, Product** products, int productCount);
    Sale** (*list)(struct SalesController* self);
    Sale* (*find)(struct SalesController* self, int id);
    Sale* (*update)(struct SalesController* self, Sale* sale);
    Sale* (*destroy)(struct SalesController* self, int id);
    int (*count)(struct SalesController* self);
} SalesController;

SalesController* salesController_new(Config* config);

void sales_save(Sale* sale, Metadata* metadata);
Sale** sales_load(Metadata* metadata);
void sales_freeList(Sale** saleList, int length);
Sale* sale_new(int userId, Product** products, int productCount);

#endif
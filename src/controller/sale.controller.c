#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "linkedlist.h"
#include "sale.model.h"
#include "sale.controller.h"
#include "product.h"

#define ENTITY "sale"

static SaleController* instance = NULL;

static double calculateTotal(SaleController* self, LinkedList* products) {
    double total = 0.0;
    Node* current = products->first; 

    while (current != NULL) {
        Product* product = (Product*)current->value;
        total += product->price * product->quantity;
        current = current->next;
    }

    return total;
}

static Sale* create(
    SaleController* self,
    int customerId,
    LinkedList* products
) {
    SaleModel* model = saleModel_new();
    Sale* newSale = model->new(customerId, 0);

    newSale->total = calculateTotal(self, products);

    return self->repository->save(newSale);
}

static Sale** list(SaleController* self) {
    return self->repository->findAll();
}

static Sale* find(SaleController* self, int id) {
    return self->repository->findById(id);
}

static Sale* destroy(SaleController* self, int id) {
    return self->repository->remove(id);
}

static int count(SaleController* self) {
    return self->repository->count();
}

SaleController* saleController_new(Config* config) {
    if (instance != NULL) {
        return instance;
    }

    SaleController* controller = (SaleController*)malloc(sizeof(SaleController));

    Metadata* metadata = config_getMetadata(config, ENTITY);
    if (metadata == NULL) {
        fprintf(stderr, "\n[Erro]: Não foi possível encontrar os dados para vendas.\n");
        exit(EXIT_FAILURE);
    }

    controller->repository = saleRepository_new(metadata);
    controller->create = create;
    controller->list = list;
    controller->find = find;
    controller->destroy = destroy;
    controller->count = count;

    return controller;
}

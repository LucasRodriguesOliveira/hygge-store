#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "sales.model.h"
#include "sales.controller.h"

#define ENTITY "sales"

static SalesController* instance = NULL;

static Sales* create(SalesController* self, int customerId, LinkedList* products, char* name) {
    SalesModel* model = salesModel_new();
    Sales* newSale = model->new(customerId, products, name);
    newSale->total = self->calculateTotal(self, products);
    return self->repository->save(newSale);
}

static Sales** list(SalesController* self) {
    return self->repository->findAll();
}

static Sales* find(SalesController* self, int id) {
    return self->repository->findById(id);
}

static Sales* update(SalesController* self, Sales* sales) {
    sales->total = self->calculateTotal(self, sales->products); /
    return self->repository->update(sales);
}

static Sales* destroy(SalesController* self, int id) {
    return self->repository->remove(id);
}

static int count(SalesController* self) {
    return self->repository->count();
}

static double calculateTotal(SalesController* self, LinkedList* products) {
    double total = 0.0;
    Node* current = products->first; 

    while (current != NULL) {
        Product* product = (Product*)current->value;
        total += product->price * product->quantity;
        current = current->next;
    }

    return total;
}

SalesController* salesController_new(Config* config) {
    if (instance != NULL) {
        return instance;
    }

    SalesController* controller = (SalesController*)malloc(sizeof(SalesController));

    Metadata* metadata = config_getMetadata(config, ENTITY);
    if (metadata == NULL) {
        fprintf(stderr, "\n[Erro]: Não foi possível encontrar os dados para vendas.\n");
        exit(EXIT_FAILURE);
    }

    controller->repository = salesRepository_new(metadata);
    controller->create = create;
    controller->list = list;
    controller->find = find;
    controller->update = update;
    controller->destroy = destroy;
    controller->count = count;
    controller->calculateTotal = calculateTotal;

    return controller;
}

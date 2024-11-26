#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "customer.model.h"
#include "customer.controller.h"

#define ENTITY "customer"

static CustomerController* instance = NULL;

static Customer* create(CustomerController* self, char* name) {
  CustomerModel* model = customerModel_new();
  return self->repository->save(model->new(name));
}

static Customer** list(CustomerController* self) {
  return self->repository->findAll();
}

static Customer* find(CustomerController* self, int id) {
  return self->repository->findById(id);
}

static Customer* update(CustomerController* self, Customer* customer) {
  return self->repository->update(customer);
}

static Customer* destroy(CustomerController* self, int id) {
  return self->repository->remove(id);
}

static int count(CustomerController* self) {
  return self->repository->count();
}

CustomerController* customerController_new(Config* config) {
  if (instance != NULL) {
    return instance;
  }

  CustomerController* controller =
    (CustomerController*)malloc(sizeof(CustomerController));

  Metadata* metadata = config_getMetadata(config, ENTITY);
  if (metadata == NULL) {
    fprintf(stderr, "\n[Erro]: Não foi possível encontrar os dados.\n");
    exit(EXIT_FAILURE);
  }

  controller->repository = customerRepository_new(metadata);
  controller->create = create;
  controller->list = list;
  controller->find = find;
  controller->update = update;
  controller->destroy = destroy;
  controller->count = count;

  return controller;
}

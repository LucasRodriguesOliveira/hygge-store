#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "metadata.h"
#include "customer.h"
#include "customer.controller.h"
#include "customer.view.h"

static void list(Config* config) {
  CustomerController* controller = customerController_new(config);
  CustomerModel* model = customerModel_new();
  int length = controller->count(controller);

  if (length == 0) {
    printf("[WRN]: Não há clientes cadastrados.\n");
    return;
  }

  Customer** customers = controller->list(controller);

  printf("Total: %d\n", length);
  model->printList(customers, length);

  model->freeList(customers, length);
}

static void create(Config* config) {
  CustomerController* controller = customerController_new(config);
  CustomerModel* model = customerModel_new();
  char name[100];

  printf("Nome: ");
  scanf(" %100[^\n]", name);

  Customer* customer = controller->create(controller, name);

  printf("\n[INF]: Cliente salvo.\n");

  model->columnsToString();
  model->toString(customer);

  free(customer);
}

static void find(Config* config) {
  CustomerController* controller = customerController_new(config);
  CustomerModel* model = customerModel_new();
  int id;

  int length = controller->count(controller);

  if (length == 0) {
    printf("[WRN]: Não há clientes cadastrados\n");
    return;
  }

  printf("Digite o ID do cliente: ");
  scanf("%d", &id);

  Customer* customer = controller->find(controller, id);

  if (customer == NULL) {
    printf("\n[WRN]: Cliente não encontrado\n");
    return;
  }

  printf("\n[INF]: Cliente encontrado.\n");

  model->columnsToString();
  model->toString(customer);

  free(customer);
}

static void delete(Config* config) {
  CustomerController* controller = customerController_new(config);
  CustomerModel* model = customerModel_new();

  int length = controller->count(controller);

  if (length == 0) {
    printf("[WRN]: Não há clientes cadastrados\n");
    return;
  }

  int id;

  printf("Digite o ID do cliente: ");
  scanf("%d", &id);

  Customer* customer = controller->destroy(controller, id);

  if (customer == NULL) {
    printf("\n[WRN]: Cliente não encontrado.\n");
    return;
  }

  printf("\n[INF]: Cliente removido.\n");

  model->columnsToString();
  model->toString(customer);

  free(customer);
}

static void update(Config* config) {
  CustomerController* controller = customerController_new(config);
  CustomerModel* model = customerModel_new();

  int length = controller->count(controller);

  if (length == 0) {
    printf("[WRN]: Não há clientes cadastrados\n");
    return;
  }

  int id;

  printf("Digite o ID do cliente: ");
  scanf("%d", &id);

  Customer* customer = controller->find(controller, id);

  if (customer == NULL) {
    printf("\n[WRN]: cliente não encontrado.\n");
    return;
  }

  model->columnsToString();
  model->toString(customer);

  printf("\nNovo nome: ");
  char name[100];
  scanf(" %100[^\n]", name);
  snprintf(customer->name, 100, "%s", name);

  customer = controller->update(controller, customer);

  if (customer == NULL) {
    fprintf(stderr, "\n\[ERR]: Não foi possível atualizar o cliente.\n");
    return;
  }

  printf("\n[INF]: Cliente atualizado.\n");

  model->columnsToString();
  model->toString(customer);

  free(customer);
}

View* customerView_new(Config* config) {
  View* customerView = view_new(config, "Cliente", "Voltar");

  customerView->addOption(
    customerView,
    viewoption_new(
      "Listar clientes",
      list
    )
  );

  customerView->addOption(
    customerView,
    viewoption_new(
      "Cadastrar cliente",
      create
    )
  );

  customerView->addOption(
    customerView,
    viewoption_new(
      "Buscar cliente por id",
      find
    )
  );

  customerView->addOption(
    customerView,
    viewoption_new(
      "Atualizar cliente",
      update
    )
  );

  customerView->addOption(
    customerView,
    viewoption_new(
      "Remover cliente",
      delete
    )
  );

  return customerView;
}

#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "linkedlist.h"
#include "node.h"
#include "metadata.h"
#include "category.controller.h"
#include "product.controller.h"
#include "product.model.h"
#include "customer.controller.h"
#include "customer.model.h"
#include "sale.h"
#include "sale.controller.h"
#include "sale.view.h"

static void list(Config* config) {
  SaleController* controller = saleController_new(config);
  CustomerController* customerController = customerController_new(config);
  SaleModel* model = saleModel_new();
  int length = controller->count(controller);

  if (length == 0) {
    printf("[WRN]: Não há vendas cadastradas.\n");
    return;
  }

  Sale** sales = controller->list(controller);

  printf("Total: %d\n", length);
  model->printList(sales, length, customerController->repository->findById);

  model->freeList(sales, length);
}

static void create(Config* config) {
  SaleController* controller = saleController_new(config);
  CustomerController* customerController = customerController_new(config);
  ProductController* productController = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);

  SaleModel* model = saleModel_new();
  CustomerModel* customerModel = customerModel_new();
  ProductModel* productModel = productModel_new();

  int customerLength = customerController->count(customerController);

  if (customerLength == 0) {
    printf("[WRN]: Não há clientes cadastrados para efetuar uma venda.\n");
    return;
  }

  int productLength = productController->count(productController);

  if (productLength == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  Customer** customerList = customerController->list(customerController);
  customerModel->printList(customerList, customerLength);
  customerModel->freeList(customerList, customerLength);

  int customerId;
  printf("Id do cliente: ");
  scanf(" %d", &customerId);

  Customer* customer = customerController->find(customerController, customerId);

  if (customer == NULL) {
    printf("[ERR]: Cliente não encontrado.\n");
    return;
  }

  Product** productList = productController->list(productController);

  int option = -1;
  LinkedList* productsToBuy = linkedlist_new();

  do {
    productModel->printList(
      productList,
      productLength,
      categoryController->repository->findById
    );

    printf("Id do produto (0 para sair): ");
    scanf(" %d", &option);

    if (option != 0) {
      Product* product = productController->find(productController, option);

      if (product == NULL) {
        printf("[ERR]: Não foi possível encontrar o produto.\n");
        productModel->freeList(productList, productLength);
        linkedlist_free(productsToBuy, 1);
        return;
      }

      linkedlist_add(productsToBuy, node_new(product));
    }
  } while (option != 0);
  productModel->freeList(productList, productLength);

  if (productsToBuy->length == 0) {
    printf("[WRN]: Pelo menos um produto deve ser selecionado.\n");
    linkedlist_free(productsToBuy, 1);
    return;
  }

  Sale* sale = controller->create(controller, customerId, productsToBuy);
  linkedlist_free(productsToBuy, 1);

  printf("\n[INF]: Venda de R$ %8.2lf registrada.\n", sale->total);

  model->toString(sale, customer);

  free(sale);
  free(customer);
}

static void find(Config* config) {
  SaleController* controller = saleController_new(config);
  CustomerController* customerController = customerController_new(config);
  SaleModel* model = saleModel_new();
  int id;

  int length = controller->count(controller);

  if (length == 0) {
    printf("[WRN]: Não há vendas registradas\n");
    return;
  }

  printf("Digite o ID da venda: ");
  scanf("%d", &id);

  Sale* sale = controller->find(controller, id);

  if (sale == NULL) {
    printf("\n[WRN]: Venda não encontrada\n");
    return;
  }

  printf("\n[INF]: Venda encontrada.\n");

  Customer* customer = customerController->find(customerController, sale->customerId);

  if (customer == NULL) {
    printf("[ERR]: Erro ao procurar cliente da venda -> cliente não encontrado\n");
    free(sale);
    return;
  }

  model->toString(sale, customer);

  free(sale);
  free(customer);
}

View* saleView_new(Config* config) {
  View* saleView = view_new(config, "Venda", "Voltar");

  saleView->addOption(
    saleView,
    viewoption_new(
      "Listar vendas",
      list
    )
  );

  saleView->addOption(
    saleView,
    viewoption_new(
      "Cadastrar venda",
      create
    )
  );

  saleView->addOption(
    saleView,
    viewoption_new(
      "Buscar venda por id",
      find
    )
  );

  return saleView;
}

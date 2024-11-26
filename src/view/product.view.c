#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "metadata.h"
// #include "binarytree.h"
#include "product.h"
#include "product.controller.h"
#include "product.view.h"

static void list(Config* config) {
  ProductController* controller = productController_new(config);
  ProductModel* model = productModel_new();
  Product** products = controller->list(controller);
  int length = controller->count(controller);

  printf("Total: %d\n", length);
  model->printList(products, length);

  model->freeList(products, length);
}

static void create(Config* config) {
  ProductController* controller = productController_new(config);
  ProductModel* model = productModel_new();
  char name[100];

  printf("Nome: ");
  scanf(" %100[^\n]", name);

  Product* product = controller->create(controller, name);

  printf("\n[INF]: Produto salvo.\n");

  model->columnsToString();
  model->toString(product);

  free(product);
}

static void find(Config* config) {
  ProductController* controller = productController_new(config);
  ProductModel* model = productModel_new();
  int id;

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: Produto não encontrado\n");
    return;
  }

  printf("\n[INF]: Produto encontrado.\n");

  model->columnsToString();
  model->toString(product);

  free(product);
}

static void delete(Config* config) {
  ProductController* controller = productController_new(config);
  ProductModel* model = productModel_new();
  int id;

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->destroy(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: Produto não encontrado.\n");
    return;
  }

  printf("\n[INF]: Produto removido.\n");

  model->columnsToString();
  model->toString(product);

  free(product);
}

static void update(Config* config) {
  ProductController* controller = productController_new(config);
  ProductModel* model = productModel_new();
  int id;

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: produto não encontrado.\n");
    return;
  }

  model->columnsToString();
  model->toString(product);

  printf("\nNovo nome: ");
  char name[100];
  scanf(" %100[^\n]", name);
  snprintf(product->name, 100, "%s", name);

  product = controller->update(controller, product);

  if (product == NULL) {
    fprintf(stderr, "\n\[ERR]: Não foi possível atualizar o produto.\n");
    return;
  }

  printf("\n[INF]: Produto atualizado.\n");

  model->columnsToString();
  model->toString(product);

  free(product);
}

View* productView_new(Config* config) {
  View* productView = view_new(config, "Produto", "Voltar");

  productView->addOption(
    productView,
    viewoption_new(
      "Listar produtos",
      list
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Cadastrar produto",
      create
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Buscar produto por id",
      find
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Atualizar produto",
      update
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Remover produto",
      delete
    )
  );

  return productView;
}

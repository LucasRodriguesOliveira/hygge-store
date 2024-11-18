#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "metadata.h"
#include "binarytree.h"
#include "product.h"
#include "product.controller.h"
#include "product.view.h"

static void list(Config* config) {
  ProductController* controller = productController_new(config);
  Product** products = controller->list(controller);
  int length = controller->count(controller);

  printf("Total: %d\n", length);
  printf("\n| ID\t| Name |\n");
  printf("---------------------------------\n");
  for (int i = 0; i < length; i++) {
    Product* p = products[i];

    printf("%d\t| %s\n", p->id, p->name);
  }

  product_freeList(products, length);
  free(controller);
}

static void create(Config* config) {
  ProductController* controller = productController_new(config);
  char name[100];

  printf("Nome: ");
  scanf(" %100[^\n]", name);

  Product* product = controller->create(controller, name);
  printf("[INF]: Produto salvo.\n");
  printf("| Id\t| Name\n");
  printf("-------------------------\n");
  printf("| %d\t| %s\n", product->id, product->name);

  free(product);
  free(controller);
}

static void find(Config* config) {
  ProductController* controller = productController_new(config);
  int id;

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("[WRN]: Produto não encontrado\n");
    return;
  }

  printf("[INF]: Produto encontrado:\n");
  printf("| Id\t| Name\n");
  printf("-------------------------\n");
  printf("| %d\t| %s\n", product->id, product->name);

  free(product);
  free(controller);
}

static void delete(Config* config) {
  ProductController* controller = productController_new(config);
  int id;

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->destroy(controller, id);

  if (product == NULL) {
    printf("[WRN]: Produto não encontrado.\n");
    return;
  }

  printf("[INF]: Produto removido.\n");
  printf("| Id\t| Name\n");
  printf("-------------------------\n");
  printf("| %d\t| %s\n", product->id, product->name);

  free(product);
  free(controller);
}

static void update(Config* config) {
  ProductController* controller = productController_new(config);
  int id;

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);
  printf("| Id\t| Name\n");
  printf("-------------------------\n");
  printf("| %d\t| %s\n", product->id, product->name);

  printf("Novo nome: ");
  char name[100];
  scanf(" %100[^\n]", name);
  snprintf(product->name, 100, "%s", name);

  product = controller->update(controller, product);

  printf("[INF]: Produto atualizado.\n");
  printf("| Id\t| Name\n");
  printf("-------------------------\n");
  printf("| %d\t| %s\n", product->id, product->name);

  free(product);
  free(controller);
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

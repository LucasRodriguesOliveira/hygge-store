#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "node.h"
#include "linkedlist.h"
#include "metadata.h"
#include "product.h"
#include "category.h"
#include "category.model.h"
#include "product.model.h"
#include "category.controller.h"
#include "category.view.h"

static void list(Config* config) {
  CategoryController* controller = categoryController_new(config);
  CategoryModel* model = categoryModel_new();
  Category** categories = controller->list(controller);
  int length = controller->count(controller);

  printf("Total: %d\n", length);
  model->printList(categories, length);

  model->freeList(categories, length);
}

static void listProducts(Config* config) {
  CategoryController* controller = categoryController_new(config);
  CategoryModel* model = categoryModel_new();
  ProductModel* productModel = productModel_new();

  int productLength = controller->productRepository->count();

  if (productLength == 0) {
    printf("[WRN] Não há produtos cadastrados.\n");
    return;
  }

  Category** categories = controller->list(controller);
  int length = controller->count(controller);

  model->printList(categories, length);
  model->freeList(categories, length);

  int id;
  printf("\nId da categoria -> ");
  scanf(" %d", &id);

  Category* category = controller->find(controller, id);

  if (category == NULL) {
    printf("\n[WRN]: Categoria não encontrada\n");
    return;
  }

  LinkedList* productList = controller->listProductsByCategory(controller, id);

  if (productList == NULL) {
    printf("[WRN]: Nenhum produto com esta categoria\n");
    return;
  }

  Node* node = productList->first;
  while (node != NULL) {
    productModel->toString((Product*) node->value, category);
    node = node->next;
  }

  linkedlist_free(productList, 1);
  free(category);
}

static void create(Config* config) {
  CategoryController* controller = categoryController_new(config);
  CategoryModel* model = categoryModel_new();
  char description[100];

  printf("Descrição: ");
  scanf(" %100[^\n]", description);

  Category* category = controller->create(controller, description);

  printf("\n[INF]: Categoria salvo.\n");

  model->columnsToString();
  model->toString(category);

  free(category);
}

static void find(Config* config) {
  CategoryController* controller = categoryController_new(config);
  CategoryModel* model = categoryModel_new();
  int id;

  printf("Digite o ID do categoria: ");
  scanf("%d", &id);

  Category* category = controller->find(controller, id);

  if (category == NULL) {
    printf("\n[WRN]: Categoria não encontrado\n");
    return;
  }

  printf("\n[INF]: Categoria encontrado.\n");

  model->columnsToString();
  model->toString(category);

  free(category);
}

static void delete(Config* config) {
  CategoryController* controller = categoryController_new(config);
  CategoryModel* model = categoryModel_new();
  int id;

  printf("Digite o ID do categoria: ");
  scanf("%d", &id);

  Category* category = controller->destroy(controller, id);

  if (category == NULL) {
    printf("\n[WRN]: Categoria não encontrado.\n");
    return;
  }

  printf("\n[INF]: Categoria removido.\n");

  model->columnsToString();
  model->toString(category);

  free(category);
}

static void update(Config* config) {
  CategoryController* controller = categoryController_new(config);
  CategoryModel* model = categoryModel_new();
  int id;

  printf("Digite o ID do categoria: ");
  scanf("%d", &id);

  Category* category = controller->find(controller, id);

  if (category == NULL) {
    printf("\n[WRN]: categoria não encontrado.\n");
    return;
  }

  model->columnsToString();
  model->toString(category);

  printf("\nNova descrição: ");
  char description[100];
  scanf(" %100[^\n]", description);
  snprintf(category->description, 100, "%s", description);

  category = controller->update(controller, category);

  if (category == NULL) {
    fprintf(stderr, "\n\[ERR]: Não foi possível atualizar o categoria.\n");
    return;
  }

  printf("\n[INF]: Categoria atualizado.\n");

  model->columnsToString();
  model->toString(category);

  free(category);
}

View* categoryView_new(Config* config) {
  View* categoryView = view_new(config, "Categoria", "Voltar");

  categoryView->addOption(
    categoryView,
    viewoption_new(
      "Listar categorias",
      list
    )
  );

  categoryView->addOption(
    categoryView,
    viewoption_new(
      "Listar produtos por categoria",
      listProducts
    )
  );

  categoryView->addOption(
    categoryView,
    viewoption_new(
      "Cadastrar categoria",
      create
    )
  );

  categoryView->addOption(
    categoryView,
    viewoption_new(
      "Buscar categoria por id",
      find
    )
  );

  categoryView->addOption(
    categoryView,
    viewoption_new(
      "Atualizar categoria",
      update
    )
  );

  categoryView->addOption(
    categoryView,
    viewoption_new(
      "Remover categoria",
      delete
    )
  );

  return categoryView;
}

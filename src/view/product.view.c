#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "metadata.h"
#include "category.controller.h"
#include "category.model.h"
#include "product.h"
#include "product.model.h"
#include "product.controller.h"
#include "product.view.h"

static void list(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  Product** products = controller->list(controller);
  int length = controller->count(controller);

  printf("Total: %d\n", length);
  model->printList(products, length, categoryController->repository->findById);

  model->freeList(products, length);
}

static void create(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  CategoryModel* categoryModel = categoryModel_new();

  int count = categoryController->count(categoryController);

  if (count == 0) {
    printf("[WRN]: Não há categorias cadastradas.\n");
    return;
  }

  char name[100];
  double price;
  int quantity;
  int categoryId;

  printf("Nome: ");
  scanf(" %100[^\n]", name);

  printf("Preço: ");
  scanf(" %lf", &price);

  printf("Quantidade: ");
  scanf(" %d", &quantity);

  Category** categoryList = categoryController->list(categoryController);
  int categoryListLength = categoryController->count(categoryController);

  printf("Categorias disponíveis:\n");
  categoryModel->printList(categoryList, categoryListLength);

  printf("\nCategoria: ");
  scanf(" %d", &categoryId);

  Category* category = categoryController->find(categoryController, categoryId);

  if (category == NULL) {
    printf("[ERR]: Categoria não encontrada\n");
  } else {
    Product* product = controller->create(controller, name, price, quantity, category->id);

    printf("\n[INF]: Produto salvo.\n");

    model->toString(product, category);

    free(product);
  }

  categoryModel->freeList(categoryList, categoryListLength);
  free(category);
}

static void find(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  int id;

  int count = controller->count(controller);

  if (count == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: Produto não encontrado\n");
    return;
  }

  Category* category = categoryController->find(
    categoryController,
    product->categoryId
  );

  if (category == NULL) {
    CategoryModel* categoryModel = categoryModel_new();
    category = categoryModel->new("Sem categoria");
  }

  printf("\n[INF]: Produto encontrado.\n");

  model->toString(product, category);

  free(product);
  free(category);
}

static void delete(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  int id;

  int count = controller->count(controller);

  if (count == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->destroy(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: Produto não encontrado.\n");
    return;
  }

  Category* category = categoryController->find(
    categoryController,
    product->categoryId
  );

  if (category == NULL) {
    CategoryModel* categoryModel = categoryModel_new();
    category = categoryModel->new("Sem categoria");
  }

  printf("\n[INF]: Produto removido.\n");

  model->toString(product, category);

  free(product);
  free(category);
}

static void updateName(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  int id;

  int count = controller->count(controller);

  if (count == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: produto não encontrado.\n");
    return;
  }

  Category* category = categoryController->find(
    categoryController,
    product->categoryId
  );

  if (category == NULL) {
    CategoryModel* categoryModel = categoryModel_new();
    category = categoryModel->new("Sem categoria");
  }

  model->toString(product, category);

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

  model->toString(product, category);

  free(product);
  free(category);
}

static void updatePrice(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  int id;

  int count = controller->count(controller);

  if (count == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: produto não encontrado.\n");
    return;
  }

  Category* category = categoryController->find(
    categoryController,
    product->categoryId
  );

  if (category == NULL) {
    CategoryModel* categoryModel = categoryModel_new();
    category = categoryModel->new("Sem categoria");
  }

  model->toString(product, category);

  printf("\nNovo preço: ");
  double price;
  scanf(" %lf", &price);
  product->price = price;

  product = controller->update(controller, product);

  if (product == NULL) {
    fprintf(stderr, "\n\[ERR]: Não foi possível atualizar o produto.\n");
    return;
  }

  printf("\n[INF]: Produto atualizado.\n");

  model->toString(product, category);

  free(product);
  free(category);
}

static void updateQuantity(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  int id;

  int count = controller->count(controller);

  if (count == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: produto não encontrado.\n");
    return;
  }

  Category* category = categoryController->find(
    categoryController,
    product->categoryId
  );

  if (category == NULL) {
    CategoryModel* categoryModel = categoryModel_new();
    category = categoryModel->new("Sem categoria");
  }

  model->toString(product, category);

  printf("\nNova quantidade: ");
  int quantity;
  scanf(" %d", &quantity);
  product->quantity = quantity;

  product = controller->update(controller, product);

  if (product == NULL) {
    fprintf(stderr, "\n\[ERR]: Não foi possível atualizar o produto.\n");
    return;
  }

  printf("\n[INF]: Produto atualizado.\n");

  model->toString(product, category);

  free(product);
  free(category);
}

static void updateCategory(Config* config) {
  ProductController* controller = productController_new(config);
  CategoryController* categoryController = categoryController_new(config);
  ProductModel* model = productModel_new();
  CategoryModel* categoryModel = categoryModel_new();
  int id;

  int count = controller->count(controller);

  if (count == 0) {
    printf("[WRN]: Não há produtos cadastrados.\n");
    return;
  }

  int categoryCount = categoryController->count(categoryController);

  if (categoryCount == 0) {
    printf("[WRN]: Não há categorias cadastradas.\n");
    return;
  }

  printf("Digite o ID do produto: ");
  scanf("%d", &id);

  Product* product = controller->find(controller, id);

  if (product == NULL) {
    printf("\n[WRN]: produto não encontrado.\n");
    return;
  }

  Category* category = categoryController->find(
    categoryController,
    product->categoryId
  );

  if (category == NULL) {
    CategoryModel* categoryModel = categoryModel_new();
    category = categoryModel->new("Sem categoria");
  }

  model->toString(product, category);
  free(category);

  Category** categoryList = categoryController->list(categoryController);
  int categoryListLength = categoryController->count(categoryController);

  printf("Categorias disponíveis:\n");
  categoryModel->printList(categoryList, categoryListLength);

  int categoryId;
  printf("\nNova categoria: ");
  scanf(" %d", &categoryId);

  category = categoryController->find(categoryController, categoryId);

  if (category == NULL) {
    printf("[ERR]: Categoria não encontrada\n");
  } else {
    product->categoryId = categoryId;

    product = controller->update(controller, product);

    if (product == NULL) {
      fprintf(stderr, "\n\[ERR]: Não foi possível atualizar o produto.\n");
      return;
    }

    printf("\n[INF]: Produto atualizado.\n");

    model->toString(product, category);

    free(product);
  }

  categoryModel->freeList(categoryList, categoryListLength);
  free(category);
}

static void updateMenu(Config* config) {
  View* updateView = view_new(config, "Produto > Atualizar", "Voltar");

  updateView->addOption(
    updateView,
    viewoption_new(
      "Nome",
      updateName
    )
  );

  updateView->addOption(
    updateView,
    viewoption_new(
      "Preço",
      updatePrice
    )
  );

  updateView->addOption(
    updateView,
    viewoption_new(
      "Quantidade",
      updateQuantity
    )
  );

  updateView->addOption(
    updateView,
    viewoption_new(
      "Categoria",
      updateCategory
    )
  );

  updateView->show(updateView);
  view_free(updateView);
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
      updateMenu
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

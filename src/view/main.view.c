#include "config.h"
#include "view.h"
#include "category.view.h"
#include "customer.view.h"
#include "product.view.h"
#include "sale.view.h"

void productMenu(Config* config) {
  View* productView = productView_new(config);

  productView->show(productView);

  view_free(productView);
}

void categoryMenu(Config* config) {
  View* categoryView = categoryView_new(config);

  categoryView->show(categoryView);

  view_free(categoryView);
}

void customerMenu(Config* config) {
  View* customerView = customerView_new(config);

  customerView->show(customerView);

  view_free(customerView);
}

void saleMenu(Config* config) {
  View* saleView = saleView_new(config);

  saleView->show(saleView);

  view_free(saleView);
}

View* mainView_new(Config* config) {
  View* productView = view_new(
    config,
    "Principal",
    "Finalizar aplicação"
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Vendas",
      saleMenu
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Produtos",
      productMenu
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Clientes",
      customerMenu
    )
  );

  productView->addOption(
    productView,
    viewoption_new(
      "Categorias",
      categoryMenu
    )
  );

  return productView;
}
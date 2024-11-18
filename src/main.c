#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "view.h"
#include "product.view.h"

int main() {
  Config* config = config_new();
  View* productView = productView_new(config);

  printf("\n[Hygge Store]\n");
  productView->show(productView);

  free(productView);
  config_free(config);

  return 0;
}

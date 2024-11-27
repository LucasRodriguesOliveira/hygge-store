#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "view.h"
#include "main.view.h"

int main() {
  Config* config = config_new();
  View* mainView = mainView_new(config);

  printf("\n[Hygge Store]\n");
  mainView->show(mainView);

  view_free(mainView);
  config_free(config);

  return 0;
}

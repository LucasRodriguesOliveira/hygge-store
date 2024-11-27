#include <stdlib.h>
#include <stdio.h>

#include "view.h"
#include "linkedlist.h"
#include "node.h"

void addOption(View* self, MenuOption* option) {
  linkedlist_add(self->options, node_new(option));
}

char* getText(Node* node) {
  MenuOption* option = (MenuOption*) node->value;

  return option->title;
}

void listOptions(View* view) {
  Node* node = view->options->first;
  int index = 0;

  while (node != NULL) {
    index++;
    char* text = view->getText(node);
    printf("-> [%d]: %s\n", index, text);

    node = node->next;
  }
}

void showOption(LinkedList* options, Config* config, int pos) {
  int index = 0;
  Node* node = options->first;

  while (node != NULL) {
    if (index == pos) {
      ((MenuOption*) node->value)->run(config);
    }

    node = node->next;
    index++;
  }
}

void show(View* self) {
  int option = -1;

  while (option != 0) {
    printf("\nMenu [%s]:\n", self->menuName);
    listOptions(self);
    printf("-> [0]: %s\n", self->exitMessage);
    printf("\n-> ");
    scanf(" %d", &option);

    if (option < 0 || option > self->options->length) {
      printf("\n[Erro]: Opção inválida\n");
      continue;
    }

    if (option == 0) {
      break;
    }

    showOption(self->options, self->config, option - 1);
  }
}


View* view_new(Config* config, char* name, char* exit) {
  View* view = (View*) malloc(sizeof(View));

  snprintf(view->menuName, 50, "%s", name);
  snprintf(view->exitMessage, 100, "%s", exit);

  view->config = config;
  view->options = linkedlist_new();
  view->addOption = addOption;
  view->getText = getText;
  view->show = show;

  return view;
}

MenuOption* viewoption_new(char* title, RunViewFn runFn) {
  MenuOption* option = (MenuOption*) malloc(sizeof(MenuOption));
  snprintf(option->title, VIEW_TITLE_SIZE, "%s", title);
  option->run = runFn;

  return option;
}

void view_free(View* view) {
  linkedlist_free(view->options, 0);
  free(view);
}

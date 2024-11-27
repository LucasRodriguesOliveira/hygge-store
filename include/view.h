#ifndef VIEW_H
#define VIEW_H

#include "config.h"
#include "prelude.h"
#include "metadata.h"
#include "linkedlist.h"

#define VIEW_TITLE_SIZE 100

typedef struct view View;
typedef void (*RunViewFn)(Config* config);
typedef struct menu_option MenuOption;

struct menu_option {
  char title[VIEW_TITLE_SIZE];
  RunViewFn run;
};

struct view {
  /** Aplication context */
  Config* config;

  /** Defines menu name to be displayed */
  char menuName[50];

  /** message to display when exiting the menu */
  char exitMessage[100];

  /** used by main or other views to display all the options and capture user choice */
  void (*show)(View* self);

  /** allow to add menu items to be displayed */
  void (*addOption)(View* self, MenuOption* option);

  /** get a option text */
  char* (*getText)(Node* node);

  LinkedList* options;
};

View* view_new(Config* config, char* name, char* exit);
MenuOption* viewoption_new(char* title, RunViewFn runFn);
void view_free(View* view);

#endif

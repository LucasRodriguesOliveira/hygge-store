#ifndef CATEGORY_H
#define CATEGORY_H

#define CATEGORY_DESCRIPTION_LENGTH 100

typedef struct category Category;

struct category {
  int id;
  char description[100];
};

#endif

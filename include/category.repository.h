#ifndef CATEGORY_REPOSITORY_H
#define CATEGORY_REPOSITORY_H

#include "metadata.h"
#include "category.h"
#include "category.model.h"

typedef struct category_repository CategoryRepository;

struct category_repository {
  Metadata* metadata;
  CategoryModel* model;

  Category* (*save)(Category* category);
  Category** (*findAll)(void);
  Category* (*findById)(int id);
  Category* (*update)(Category* category);
  Category* (*remove)(int id);
  int (*count)(void);
};

CategoryRepository* categoryRepository_new(Metadata* metadata);

#endif

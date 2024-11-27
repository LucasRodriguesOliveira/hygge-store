#include <stdlib.h>

#include "avltree.h"
#include "metadata.h"
#include "category.repository.h"
#include "category.model.h"
#include "category.h"

static CategoryRepository* instance = NULL;

static void saveData(Category* category) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(category, sizeof(Category), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

static Category* save(Category* category) {
  category->id = instance->metadata->nextId++;
  saveData(category);
  return category;
}

static void override(Category** categoryList) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_WRITE
  );

  int length = metadata->count;

  for (int i = 0; i < length; i++) {
    fwrite(categoryList[i], sizeof(Category), 1, file);
  }

  fclose(file);
}

static Category** findAll() {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  Category** categorys = (Category**)malloc(length * sizeof(Category*));

  for (int i = 0; i < length; i++) {
    categorys[i] = (Category*)malloc(sizeof(Category));
    fread(categorys[i], sizeof(Category), 1, file);
  }

  fclose(file);

  return categorys;
}

static AVLTree* listAsAVLTree() {
  Metadata* metadata = instance->metadata;
  CategoryModel* model = instance->model;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  AVLTree* categoryTree = NULL;

  for (int i = 0; i < length; i++) {
    Category* category = (Category*)malloc(sizeof(Category));

    fread(category, sizeof(Category), 1, file);

    categoryTree = avl_add(
      categoryTree,
      category,
      model->compare
    );
  }

  fclose(file);
  return categoryTree;
}

static Category* findById(int id) {
  CategoryModel* model = instance->model;
  AVLTree* categoryTree = listAsAVLTree();

  Category* found = (Category*)avl_find(
    categoryTree,
    model->create(id, ""),
    model->compare
  );

  Category* category = NULL;

  if (found != NULL) {
    category = model->copy(found);
  }

  avl_free(categoryTree);
  return category;
}

static Category* destroy(int id) {
  CategoryModel* model = instance->model;
  Metadata* metadata = instance->metadata;
  AVLTree* categoryTree = listAsAVLTree();
  Category* categoryToFind = model->create(id, "");

  Category* removed = (Category*)avl_find(
    categoryTree,
    categoryToFind,
    model->compare
  );

  if (removed == NULL) {
    avl_free(categoryTree);
    free(categoryToFind);
    return NULL;
  }

  categoryTree = avl_remove(
    categoryTree,
    categoryToFind,
    model->compare
  );

  free(categoryToFind);

  int categoryListLength;
  Category** categoryList = (Category**) avl_to_array(
    categoryTree,
    &categoryListLength
  );

  metadata->count = categoryListLength;

  override(categoryList);
  metadata_save(metadata);

  Category* category = model->copy(removed);
  avl_free(categoryTree);
  free(removed);

  return category;
}

static int count(void) {
  return instance->metadata->count;
}

static Category* update(Category* category) {
  Category* updated = destroy(category->id);

  if (updated == NULL) {
    return NULL;
  }

  snprintf(updated->description, 100, "%s", category->description);
  saveData(updated);

  free(category);
  category = instance->model->copy(updated);

  return category;
}

CategoryRepository* categoryRepository_new(Metadata* metadata) {
  if (instance != NULL) {
    return instance;
  }

  instance = (CategoryRepository*) malloc(sizeof(CategoryRepository));
  instance->model = categoryModel_new();
  instance->metadata = metadata;
  instance->save = save;
  instance->findAll = findAll;
  instance->findById = findById;
  instance->remove = destroy;
  instance->update = update;
  instance->count = count;

  return instance;
}

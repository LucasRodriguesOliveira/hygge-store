#include <stdlib.h>

#include "avltree.h"
#include "metadata.h"
#include "sale.repository.h"
#include "sale.model.h"
#include "sale.h"

static SaleRepository* instance = NULL;

static void saveData(Sale* sale) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(sale, sizeof(Sale), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

static Sale* save(Sale* sale) {
  sale->id = instance->metadata->nextId++;
  saveData(sale);
  return sale;
}

static void override(Sale** saleList) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_WRITE
  );

  int length = metadata->count;

  for (int i = 0; i < length; i++) {
    fwrite(saleList[i], sizeof(Sale), 1, file);
  }

  fclose(file);
}

static Sale** findAll() {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  Sale** sales = (Sale**)malloc(length * sizeof(Sale*));

  for (int i = 0; i < length; i++) {
    sales[i] = (Sale*)malloc(sizeof(Sale));
    fread(sales[i], sizeof(Sale), 1, file);
  }

  fclose(file);

  return sales;
}

static AVLTree* listAsAVLTree() {
  Metadata* metadata = instance->metadata;
  SaleModel* model = instance->model;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  AVLTree* saleTree = NULL;

  for (int i = 0; i < length; i++) {
    Sale* sale = (Sale*)malloc(sizeof(Sale));

    fread(sale, sizeof(Sale), 1, file);

    saleTree = avl_add(
      saleTree,
      sale,
      model->compare
    );
  }

  fclose(file);
  return saleTree;
}

static Sale* findById(int id) {
  SaleModel* model = instance->model;
  AVLTree* saleTree = listAsAVLTree();

  Sale* found = (Sale*)avl_find(
    saleTree,
    model->create(id, 0, 0),
    model->compare
  );

  Sale* sale = NULL;

  if (found != NULL) {
    sale = model->copy(found);
  }

  avl_free(saleTree);
  return sale;
}

static Sale* destroy(int id) {
  SaleModel* model = instance->model;
  Metadata* metadata = instance->metadata;
  AVLTree* saleTree = listAsAVLTree();
  Sale* saleToFind = model->create(id, 0, 0);

  Sale* removed = (Sale*)avl_find(
    saleTree,
    saleToFind,
    model->compare
  );

  if (removed == NULL) {
    avl_free(saleTree);
    free(saleToFind);
    return NULL;
  }

  saleTree = avl_remove(
    saleTree,
    saleToFind,
    model->compare
  );

  free(saleToFind);

  int saleListLength;
  Sale** saleList = (Sale**) avl_to_array(
    saleTree,
    &saleListLength
  );

  metadata->count = saleListLength;

  override(saleList);
  metadata_save(metadata);

  Sale* sale = model->copy(removed);
  avl_free(saleTree);
  free(removed);

  return sale;
}

static int count(void) {
  return instance->metadata->count;
}

static Sale* update(Sale* sale) {
  Sale* updated = destroy(sale->id);

  if (updated == NULL) {
    return NULL;
  }

  updated->customerId = sale->customerId;
  updated->total = sale->total;
  saveData(updated);

  free(sale);
  sale = instance->model->copy(updated);

  return sale;
}

SaleRepository* saleRepository_new(Metadata* metadata) {
  if (instance != NULL) {
    return instance;
  }

  instance = (SaleRepository*) malloc(sizeof(SaleRepository));
  instance->model = saleModel_new();
  instance->metadata = metadata;
  instance->save = save;
  instance->findAll = findAll;
  instance->findById = findById;
  instance->remove = destroy;
  instance->update = update;
  instance->count = count;

  return instance;
}

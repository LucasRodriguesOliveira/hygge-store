#include <stdlib.h>
#include <string.h>

#include "product.h"
#include "metadata.h"

Product* product_new(int id, char* name) {
  Product* p = (Product*) malloc(sizeof(Product));
  p->id = id;
  strncpy(p->name, name, 100);

  return p;
}

void product_save(Product* p, Metadata* metadata) {
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(p, sizeof(Product), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

Product** product_load(Metadata* metadata) {
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  Product** products = (Product**) malloc(length * sizeof(Product*));

  for (int i = 0; i < length; i++) {
    products[i] = (Product*) malloc(sizeof(Product));
    fread(products[i], sizeof(Product), 1, file);
  }

  fclose(file);

  return products;
}

void product_freeList(Product** productList, int length) {
  for (int i = 0; i < length; i++) {
    free(productList[i]);
  }

  free(productList);
}

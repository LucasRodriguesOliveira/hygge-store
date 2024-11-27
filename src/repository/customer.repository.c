#include <stdlib.h>

#include "avltree.h"
#include "metadata.h"
#include "customer.repository.h"
#include "customer.model.h"
#include "customer.h"

static CustomerRepository* instance = NULL;

static void saveData(Customer* customer) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(customer, sizeof(Customer), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

static Customer* save(Customer* customer) {
  customer->id = instance->metadata->nextId++;
  saveData(customer);
  return customer;
}

static void override(Customer** customerList) {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_WRITE
  );

  int length = metadata->count;

  for (int i = 0; i < length; i++) {
    fwrite(customerList[i], sizeof(Customer), 1, file);
  }

  fclose(file);
}

static Customer** findAll() {
  Metadata* metadata = instance->metadata;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  Customer** customers = (Customer**)malloc(length * sizeof(Customer*));

  for (int i = 0; i < length; i++) {
    customers[i] = (Customer*)malloc(sizeof(Customer));
    fread(customers[i], sizeof(Customer), 1, file);
  }

  fclose(file);

  return customers;
}

static AVLTree* listAsAVLTree() {
  Metadata* metadata = instance->metadata;
  CustomerModel* model = instance->model;
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  AVLTree* customerTree = NULL;

  for (int i = 0; i < length; i++) {
    Customer* customer = (Customer*)malloc(sizeof(Customer));

    fread(customer, sizeof(Customer), 1, file);

    customerTree = avl_add(
      customerTree,
      customer,
      model->compare
    );
  }

  fclose(file);
  return customerTree;
}

static Customer* findById(int id) {
  CustomerModel* model = instance->model;
  AVLTree* customerTree = listAsAVLTree();

  Customer* found = (Customer*)avl_find(
    customerTree,
    model->create(id, ""),
    model->compare
  );

  Customer* customer = NULL;

  if (found != NULL) {
    customer = model->copy(found);
  }

  avl_free(customerTree);
  return customer;
}

static Customer* destroy(int id) {
  CustomerModel* model = instance->model;
  Metadata* metadata = instance->metadata;
  AVLTree* customerTree = listAsAVLTree();
  Customer* customerToFind = model->create(id, "");

  Customer* removed = (Customer*)avl_find(
    customerTree,
    customerToFind,
    model->compare
  );

  if (removed == NULL) {
    avl_free(customerTree);
    free(customerToFind);
    return NULL;
  }

  customerTree = avl_remove(
    customerTree,
    customerToFind,
    model->compare
  );

  free(customerToFind);

  int customerListLength;
  Customer** customerList = (Customer**) avl_to_array(
    customerTree,
    &customerListLength
  );

  metadata->count = customerListLength;

  override(customerList);
  metadata_save(metadata);

  Customer* customer = model->copy(removed);
  avl_free(customerTree);
  free(removed);

  return customer;
}

static int count(void) {
  return instance->metadata->count;
}

static Customer* update(Customer* customer) {
  Customer* updated = destroy(customer->id);

  if (updated == NULL) {
    return NULL;
  }

  snprintf(updated->name, 100, "%s", customer->name);
  saveData(updated);

  free(customer);
  customer = instance->model->copy(updated);

  return customer;
}

CustomerRepository* customerRepository_new(Metadata* metadata) {
  if (instance != NULL) {
    return instance;
  }

  instance = (CustomerRepository*) malloc(sizeof(CustomerRepository));
  instance->model = customerModel_new();
  instance->metadata = metadata;
  instance->save = save;
  instance->findAll = findAll;
  instance->findById = findById;
  instance->remove = destroy;
  instance->update = update;
  instance->count = count;

  return instance;
}

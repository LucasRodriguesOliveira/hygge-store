#ifndef CUSTOMER_H
#define CUSTOMER_H

#define CUSTOMER_NAME_LENGTH 100

typedef struct customer Customer;

struct customer {
  int id;
  char name[100];
};

#endif

#ifndef SALES_H
#define SALES_H

#define SALES_NAME_LENGTH 100

typedef struct sales Sales;

struct sales {
  int id;
  char name[100];
};

#endif

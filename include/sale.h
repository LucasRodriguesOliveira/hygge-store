#ifndef SALES_H
#define SALES_H

typedef struct sale Sale;

struct sale {
  int id;
  int customerId;
  double total;
};

#endif

#ifndef NODE_H
#define NODE_H

typedef struct node Node;
typedef struct nodeservice NodeService;

struct node {
  void* value;
  struct node* next;
};

struct nodeservice {
  Node* (*new)(void* value);
  void (*destroy)(Node* node);
};

Node* node_new(void* value);
void node_free(Node* node);
NodeService* nodeservice_constructor();

#endif

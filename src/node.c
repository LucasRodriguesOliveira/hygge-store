#include <stdlib.h>

#include "node.h"

Node* node_new(void* value) {
  Node* node = (Node*) malloc(sizeof(Node));
  node->next = NULL;
  node->value = value;

  return node;
}

void node_free(Node* node) {
  free(node->value);
  free(node);
}

NodeService* nodeservice_constructor() {
  NodeService* service = (NodeService*) malloc(sizeof(NodeService));
  service->new = node_new;
  service->destroy = node_free;

  return service;
}

#include <stdio.h>
#include <stdlib.h>


#include "config.h"
#include "product.h"
#include "user.h"

int main() {
  Config* config = config_new();

  int option = -1;

  while (option != 0) {

    printf("Menu:\n");
    printf("[1]: Novo produto\n");
    printf("[2]: Novo usuário\n");
    printf("[3]: Listar produtos\n");
    printf("[4]: Listar usuários\n");
    printf("\n");
    printf("[0]: Sair\n");
    printf("========================================\n");

    printf("-> ");
    scanf(" %d", &option);
    int id;
    char name[100];
    Metadata* metadata;

    switch (option) {
      case 1:
        printf("Nome: ");
        scanf(" %100[^\n]", name);

        metadata = config_getMetadata(config, "product");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }
        id = metadata_nextId(metadata);

        Product* p = product_new(id, name);
        product_save(p, metadata);
        printf("Produto salvo.\n");
        free(p);
        break;
      case 2:
        printf("Nome: ");
        scanf(" %100[^\n]", name);

        metadata = config_getMetadata(config, "user");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }
        id = metadata_nextId(metadata);

        User* u = user_new(id, name);
        user_save(u, metadata);
        printf("Usuário salvo.\n");
        free(u);
        break;
      case 3:
        metadata = config_getMetadata(config, "product");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }
        Product** products = product_load(metadata);

        printf("\n| ID\t| Name |\n");
        printf("---------------------------------\n");
        for (int i = 0; i < metadata->count; i++) {
          Product* p = products[i];

          printf("%d\t| %s\n", p->id, p->name);
        }
        product_freeList(products, metadata->count);
        printf("\n");
        break;
      case 4:
        metadata = config_getMetadata(config, "user");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }
        User** users = user_load(metadata);

        printf("\n| ID\t| Name |\n");
        printf("---------------------------------\n\n");
        for (int i = 0; i < metadata->count; i++) {
          User* u = users[i];

          printf("%d\t| %s\n", u->id, u->name);
        }
        printf("\n");
        user_freeList(users, metadata->count);
        break;
    }

    metadata = NULL;
  }

  config_free(config);

  return 0;
}

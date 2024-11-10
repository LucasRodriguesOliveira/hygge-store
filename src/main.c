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
    printf("[1]: Buscar profuto por ID\n");
    printf("[0]: Sair\n");
    printf("========================================\n");

    printf("-> ");
    scanf(" %d", &option);

    int id;
    Metadata* metadata;
    Product* product;

    switch (option) {
      case 1:
        metadata = config_getMetadata(config, "product");
        if(metadata == NULL) {
          printf("Erro: dados não encontrados");
          break;
        }

        printf("Digite o ID do produto: ");
        scanf("%d", &id);

        product = product_findById(metadata, id);
        if (product != NULL) {
          printf("Produto encontrado:\n");
          printf("ID: %d, Nome %s\n", product->id, product->name);
        } else {
          printf("Produto não encontrado\n");
        }
        break;

      case 0:
        printf("Encerrado o programa.\n");
        break;

        default:
          printf("Opcao invalida. Tente novamente.\n");
     }
  }

  config_free(config);

  return 0;
}

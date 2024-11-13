#include <stdio.h>
#include <stdlib.h>

#include "binarytree.h"
#include "config.h"
#include "product.h"
#include "user.h"

int main() {
  Config* config = config_new();

  int option = -1;

  while (option != 0) {

    printf("Menu:\n");
    printf("[1]: Buscar produto por Id\n");
    printf("[2]: Cadastrar produto\n");
    printf("[3]: Listar produtos\n");
    printf("[4]: Remover produto por id\n");
    printf("[5]: Atualizar produto por id\n");
    printf("[0]: Sair\n");
    printf("========================================\n");

    printf("-> ");
    scanf(" %d", &option);

    int id;
    char name[100];
    Metadata* metadata;
    Product* product;
    BTree* productTree;

    switch (option) {
      case 1:
        metadata = config_getMetadata(config, "product");
        if(metadata == NULL) {
          printf("Erro: dados não encontrados");
          break;
        }
        productTree = product_loadAsBTree(metadata);

        printf("Digite o ID do produto: ");
        scanf("%d", &id);

        product = productTree_findById(productTree, id);
        if (product != NULL) {
          printf("Produto encontrado:\n");
          printf("ID: %d, Nome %s\n", product->id, product->name);
        } else {
          printf("Produto não encontrado\n");
        }
        break;
      case 2:
        metadata = config_getMetadata(config, "product");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }
        id = metadata->nextId++;

        printf("Nome: ");
        scanf(" %100[^\n]", name);

        product = product_new(id, name);
        product_save(product, metadata);
        printf("Produto salvo.\n");
        free(product);
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
        metadata = config_getMetadata(config, "product");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }

        productTree = product_loadAsBTree(metadata);

        printf("Digite o ID do produto: ");
        scanf("%d", &id);

        product = product_removeById(metadata, productTree, id);

        if (product == NULL) {
          printf("Produto não encontrado.\n");
          continue;
        }

        printf("Produto removido.\n");
        printf("| Id\t| Name\n");
        printf("-------------------------\n");
        printf("| %d\t| %s\n", product->id, product->name);
        btree_free(productTree);
        break;
      case 5:
        metadata = config_getMetadata(config, "product");
        if (metadata == NULL) {
          printf("Não foi possível consultar as informações: informações não encontradas.\n");
          return 1;
        }

        productTree = product_loadAsBTree(metadata);

        printf("Digite o ID do produto: ");
        scanf("%d", &id);

        product = productTree_findById(productTree, id);

        if (product == NULL) {
          printf("Produto não encontrado.\n");
          continue;
        }
        printf("Produto [%d]: %s\n", product->id, product->name);
        printf("Novo nome: ");
        scanf(" %100[^\n]", name);

        product = product_updateById(metadata, productTree, id, name);

        printf("Produto atualizado.\n");
        printf("| Id\t| Name\n");
        printf("-------------------------\n");
        printf("| %d\t| %s\n", product->id, product->name);
        btree_free(productTree);
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

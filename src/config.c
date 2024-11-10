#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "hashmap.h"
#include "binarytree.h"


#define CONFIG_PATH "./system.cfg"


Metadata* config_getMetadata(Config* config, const char* filename) {
  HashMap* map = config->metaMap;
  Metadata* metadata = metadata_new(filename);

  int hashId = metadata_hashId(metadata);
  int hash = hashmap_genHash(hashId, map->capacity);

  Node* node = hashmap_find(map, filename, hash, metadata_compare);

  if (node == NULL) {
    return NULL;
  }

  return (Metadata*) node->value;
}

char** config_filenameList(Config* config) {
  char** filenameList = (char**) malloc(
    config->metaMap->length * sizeof(char*)
  );

  Node* node = config->metaList->first;
  for (int i = 0; node != NULL; i++) {
    Node* next = node->next;
    filenameList[i] = ((Metadata*) node->value)->filename;
    node = next;
  }

  return filenameList;
}

void config_addMetadata(Config* config, Metadata* metadata) {
  hashmap_add(config->metaMap, node_new(metadata), metadata_hashId);
  linkedlist_add(config->metaList, node_new(metadata));

  config->metaArvBin = inserir(config->metaArvBin, metadata);
}

Config* config_new() {
  Config* config = (Config*) malloc(sizeof(Config));
  config->metaMap = hashmap_new();
  config->metaList = linkedlist_new();
  config->metaArvBin = NULL;

  FILE* configFile = fopen(CONFIG_PATH, "rt");

  if (configFile == NULL) {
    printf("Arquivo de configuração não encontrado!\n");
    printf("Adicione um arquivo \"%s\" na raiz do projeto\n", CONFIG_PATH);
    exit(1);
  }

  char filename[50];

  while(fscanf(configFile, " %50s[^\n]", filename) == 1) {
    Metadata* metadata = metadata_load(filename);
    config_addMetadata(config, metadata);
  }

  fclose(configFile);
  return config;
}

void config_free(Config* config) {
  hashmap_free(config->metaMap);
  linkedlist_free(config->metaList);
  liberar(config->metaArvBin);

  free(config->filename);
  free(config);
}

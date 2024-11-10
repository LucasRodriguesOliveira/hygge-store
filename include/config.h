#ifndef CONFIG_H
#define CONFIG_H

#include "binarytree.h"
#include "hashmap.h"
#include "linkedlist.h"
#include "metadata.h"

typedef struct config Config;

struct config {
  ArvBinNode* metaArvBin;
  HashMap* metaMap;
  LinkedList* metaList;
  char* filename;
};

Config* config_new();
void config_free(Config* config);
char** config_filenameList(Config* config);
void config_addMetadata(Config* config, Metadata* metadata);
Metadata* config_getMetadata(Config* config, const char* filename);

#endif

#ifndef METADATA_H
#define METADATA_H

#include <stdio.h>
#include "node.h"

typedef struct meta Metadata;
typedef enum file_mode FILE_MODE;
typedef enum file_type FILE_TYPE;

struct meta {
  int count;
  int nextId;
  char filename[50];
};

enum file_mode {
  FILE_MODE_READ,
  FILE_MODE_WRITE,
  FILE_MODE_APPEND
};

enum file_type {
  FILE_TYPE_META,
  FILE_TYPE_DATA,
};

void metadata_save(Metadata* metadata);
Metadata* metadata_load(const char* filename);
void metadata_createDatafile(Metadata* metadata);
Metadata* metadata_new(const char* filename);
int metadata_hashId(void* metadata);
char* metadata_getMetaFilename(Metadata* metadata);
char* metadata_getDataFilename(Metadata* metadata);
FILE* metadata_getFile(Metadata* metadata, FILE_TYPE type, FILE_MODE mode);
int metadata_compare(Node* node, const void* term);

#endif

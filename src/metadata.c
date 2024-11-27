#include <stdlib.h>
#include <string.h>

#include "metadata.h"
#include "hashmap.h"

#define DATA_FILE_EXT ".dat"
#define META_FILE_EXT ".cfg"
#define META_FILE_PREFIX "meta_"
#define DATA_FOLDER_PATH "./data/"

int metadata_compare(Node* node, const void* term) {
  Metadata* metadata = (Metadata*) node->value;

  return strcmp(metadata->filename, (char*) term) == 0;
}

int metadata_hashId(void* metadata) {
    const char* str = ((Metadata*) metadata)->filename;
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}


char* metadata_getMetaFilename(Metadata* meta) {
  char* metaFilename = (char*) malloc(100 * sizeof(char));
  snprintf(
    metaFilename,
    120,
    "%s%s%s%s",
    DATA_FOLDER_PATH,
    META_FILE_PREFIX,
    meta->filename,
    META_FILE_EXT
  );

  return metaFilename;
}

char* metadata_getDataFilename(Metadata* meta) {
  char* dataFilename = (char*) malloc(100 * sizeof(char));
  snprintf(
    dataFilename,
    100,
    "%s%s%s",
    DATA_FOLDER_PATH,
    meta->filename,
    DATA_FILE_EXT
  );

  return dataFilename;
}

FILE* metadata_getFile(Metadata* metadata, FILE_TYPE type, FILE_MODE mode) {
  char* filename;
  FILE* file = NULL;

  switch (type) {
    case FILE_TYPE_META:
      filename = metadata_getMetaFilename(metadata);
      break;
    case FILE_TYPE_DATA:
      filename = metadata_getDataFilename(metadata);
      break;
  }

  const char* modeStr;
  switch (mode) {
    case FILE_MODE_WRITE:
      modeStr = "wb";
      break;
    case FILE_MODE_APPEND:
      modeStr = "ab";
      break;
    case FILE_MODE_READ:
      modeStr = "rb";
      break;
    default:
      free(filename);
      return NULL;
  }

  file = fopen(filename, modeStr);

  free(filename);
  return file;
}


Metadata* metadata_new(const char* filename) {
  Metadata* metadata = (Metadata*) malloc(sizeof(Metadata));
  metadata->count = 0;
  metadata->nextId = 1;
  strncpy(metadata->filename, filename, 50);

  return metadata;
}

void metadata_createDatafile(Metadata* meta) {
  FILE* dataFile = metadata_getFile(
    meta,
    FILE_TYPE_DATA,
    FILE_MODE_WRITE
  );
  fclose(dataFile);
}

Metadata* metadata_load(const char* filename) {
  Metadata* metadata = metadata_new(filename);
  FILE* metaFile = metadata_getFile(metadata, FILE_TYPE_META, FILE_MODE_READ);

  if (metaFile == NULL) {
    // REGISTER METADATA FILE
    metaFile = metadata_getFile(metadata, FILE_TYPE_META, FILE_MODE_WRITE);
    fwrite(metadata, sizeof(Metadata), 1, metaFile);

    // REGISTER DATA FILE
    metadata_createDatafile(metadata);
  } else {
    fread(metadata, sizeof(Metadata), 1, metaFile);
  }

  fclose(metaFile);

  return metadata;
}

void metadata_save(Metadata* meta) {
  FILE* file = metadata_getFile(
    meta,
    FILE_TYPE_META,
    FILE_MODE_WRITE
  );

  fwrite(meta, sizeof(Metadata), 1, file);

  fclose(file);
}

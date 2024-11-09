#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "metadata.h"

User* user_new(int id, char* name) {
  User* u = (User*) malloc(sizeof(User));
  u->id = id;
  strncpy(u->name, name, 100);

  return u;
}

void user_save(User* u, Metadata* metadata) {
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_APPEND
  );

  fwrite(u, sizeof(User), 1, file);
  fclose(file);

  metadata->count++;
  metadata_save(metadata);
}

User** user_load(Metadata* metadata) {
  FILE* file = metadata_getFile(
    metadata,
    FILE_TYPE_DATA,
    FILE_MODE_READ
  );

  int length = metadata->count;
  User** users = (User**) malloc(length * sizeof(User*));

  for (int i = 0; i < length; i++) {
    users[i] = (User*) malloc(sizeof(User));
    fread(users[i], sizeof(User), 1, file);
  }

  fclose(file);

  return users;
}

void user_freeList(User** userList, int length) {
  for (int i = 0; i < length; i++) {
    free(userList[i]);
  }

  free(userList);
}
#ifndef USER_H
#define USER_H

#include "metadata.h"

typedef struct user User;

struct user {
  int id;
  char name[100];
};

void user_freeList(User** userList, int length);
User** user_load(Metadata* metadata);
void user_save(User* u, Metadata* metadata);
User* user_new(int id, char* name);

#endif

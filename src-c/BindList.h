#ifndef BIND_LIST_H
#define BIND_LIST_H

#include "defs.h"

struct BindList {
  struct Bind * car;
  struct BindList * cdr;
};

struct BindList * bindList_cons(struct Bind * car, struct BindList * cdr);
struct BindList * bindList_Empty();
struct BindList * bindList_push(struct BindList * list, struct Bind * datum);
int bindList_isEmpty(struct BindList * list);
struct Bind * bindList_pop(struct BindList * list);
void bindList_print(struct BindList * list);
struct Expr * bindList_find(struct Expr * key, struct BindList * list);

#endif

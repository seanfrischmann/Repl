#ifndef BIND_LIST_LIST_H
#define BIND_LIST_LIST_H

#include "defs.h"

struct BindListList {
  struct BindList * car;
  struct BindListList * cdr;
};

struct BindListList * bindListList_cons(struct BindList * car, struct BindListList * cdr);
struct BindListList * bindListList_Empty();
struct BindListList * bindListList_push(struct BindListList * list, struct BindList * datum);
int bindListList_isEmpty(struct BindListList * list);
struct BindList * bindListList_top(struct BindListList * list);
struct BindList * bindListList_pop(struct BindListList * list);
void bindListList_print(struct BindListList * list);
struct Expr * bindListList_find(struct Expr * key, struct BindListList * list);

#endif

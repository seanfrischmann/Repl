#include "BindListList.h"

/*
 * See comments in ExprList.  This is a list of BindList
 */

struct BindListList * bindListList_cons(struct BindList * car, struct BindListList * cdr) {
  struct BindListList * header;
  header = (struct BindListList *) (malloc(sizeof(struct BindListList)));
  (*header).car = car;
  (*header).cdr = cdr;
  return header;
}

struct BindListList * bindListList_Empty() {
  return bindListList_cons(NULL,NULL);
}

struct BindListList * bindListList_push(struct BindListList * list, struct BindList * datum) {
  struct BindListList * newBindListList = bindListList_cons(datum,NULL);
  (*newBindListList).cdr = (*list).cdr;
  (*list).cdr = newBindListList;
  return list;
}

int bindListList_isEmpty(struct BindListList * list) {
  return (*list).cdr == NULL;
}

struct BindList * bindListList_top(struct BindListList * list) {
  if (bindListList_isEmpty(list)) { return NULL; }
  struct BindList * top = (*(*list).cdr).car;
  return top;
}

struct BindList * bindListList_pop(struct BindListList * list) {
  if (bindListList_isEmpty(list)) { return NULL; }
  struct BindList * top = bindListList_top(list);
  (*list).cdr = (*(*list).cdr).cdr;
  return top;
}

void bindListList_print(struct BindListList * list){
  // loop through list, toString each value, print
  list = list->cdr; // skip past header node
  printf("[ ");
  while(list != NULL){
    bindList_print(list->car);
    list = list->cdr;
  }
  printf("] ");
}

struct Expr * bindListList_find(struct Expr * key, struct BindListList * list) {
  list = list->cdr; // skip past header node
  while(list != NULL){
    struct BindList * env = list->car;
    struct Expr * value = bindList_find(key,env);
    if (value != NULL) {
      return value;
    }
    list = list->cdr;
  }
  return NULL;
}

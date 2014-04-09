#include "BindList.h"

/*
 * See comments in ExprList.  This is a list of Bind
 */

struct BindList * bindList_cons(struct Bind * car, struct BindList * cdr) {
  struct BindList * header;
  header = (struct BindList *) (malloc(sizeof(struct BindList)));
  (*header).car = car;
  (*header).cdr = cdr;
  return header;
}

struct BindList * bindList_Empty() {
  return bindList_cons(NULL,NULL);
}

struct BindList * bindList_push(struct BindList * list, struct Bind * datum) {
  struct BindList * newBindList = bindList_cons(datum,NULL);
  (*newBindList).cdr = (*list).cdr;
  (*list).cdr = newBindList;
  return list;
}

int bindList_isEmpty(struct BindList * list) {
  return (*list).cdr == NULL;
}

struct Bind * bindList_pop(struct BindList * list) {
  if (bindList_isEmpty(list)) { return NULL; }
  struct Bind * top = (*(*list).cdr).car;
  (*list).cdr = (*(*list).cdr).cdr;
  return top;
}

void bindList_print(struct BindList * list){
  // loop through list, toString each value, print
  list = list->cdr; // skip past header node
  printf("( ");
  while(list != NULL){
    char * x = bind2string(list->car); // creates new char *
    printf("%s ", x);
    //    free(x); // clean up
    list = list->cdr;
  }
  printf(") ");
}

struct Expr * bindList_find(struct Expr * key, struct BindList * list) {
  list = list->cdr; // skip past header node
  while(list != NULL){
    struct Bind * pair = list->car;
    if (strcmp( nameOf(key), nameOf(pair->name) ) == 0) {
      return pair->value;
    }
    list = list->cdr;
  }
  return NULL;
}

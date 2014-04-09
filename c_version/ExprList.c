#include "ExprList.h"

/*
 * An ExprList is a list of Expr
 * It employs a header node that stores no data.
 * Insertions happen only at the front of the list (behind the header node).
 */

// builds a list node
struct ExprList * exprList_cons(struct Expr * car, struct ExprList * cdr) {
  struct ExprList * header;
  header = (struct ExprList *) (malloc(sizeof(struct ExprList)));
  (*header).car = car;
  (*header).cdr = cdr;
  return header;
}

// builds an empty list (i.e. a header node)
struct ExprList * exprList_Empty() {
  return exprList_cons(NULL,NULL);
}

// pushes at the front of the list (behind the header node)
struct ExprList * exprList_push(struct ExprList * list, struct Expr * datum) {
  struct ExprList * newExprList = exprList_cons(datum,NULL);
  (*newExprList).cdr = (*list).cdr;
  (*list).cdr = newExprList;
  return list;
}

// determines whether the list is empty (checks the cdr of the header node)
int exprList_isEmpty(struct ExprList * list) {
  return (*list).cdr == NULL;
}

// gets (but does not remove) the top/first element of the list 
struct Expr * exprList_top(struct ExprList * list) {
  if (exprList_isEmpty(list)) { return NULL; }
  struct Expr * top = (*(*list).cdr).car;
  return top;
}

// removes and return the top/first element of the list 
struct Expr * exprList_pop(struct ExprList * list) {
  if (exprList_isEmpty(list)) { return NULL; }
  struct Expr * top = exprList_top(list);
  (*list).cdr = (*(*list).cdr).cdr;
  return top;
}

// prints the contents of the list
void exprList_print(struct ExprList * list){
  // loop through list, toString each value, print
  list = list->cdr; // skip past header node
  while(list != NULL){
    char * x = expression2string(list->car); // creates new char *
    printf("%s\n",x);
    free(x); // clean up
    list = list->cdr;
  }
}

// makes a copy of the contents of the list, in reverse
// used in making a closure body
// a closure { a b c } creates a list ( c b a ), so 
// reverse is called to create ( a b c )
struct ExprList * reverseCopy(struct ExprList *list) {
  struct ExprList * rev = exprList_Empty();
  list = list->cdr; // skip past header node
  while(list != NULL){
    exprList_push(rev,list->car); // creates new char *
    list = list->cdr;
  }
  return rev;
}

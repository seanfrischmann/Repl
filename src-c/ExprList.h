#ifndef EXPR_LIST_H
#define EXPR_LIST_H

#include "defs.h"

struct ExprList {
  struct Expr * car;
  struct ExprList * cdr;
};

struct ExprList * exprList_cons(struct Expr * car, struct ExprList * cdr);
struct ExprList * exprList_Empty();
struct ExprList * exprList_push(struct ExprList * list, struct Expr * datum);
int exprList_isEmpty(struct ExprList * list);
struct Expr * exprList_top(struct ExprList * list);
struct Expr * exprList_pop(struct ExprList * list);
void exprList_print(struct ExprList *list);
struct ExprList * reverseCopy(struct ExprList *list);

#endif

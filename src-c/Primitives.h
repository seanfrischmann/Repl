#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "defs.h"

struct Expr * add(struct Expr * num1, struct Expr * num2);
struct Expr * and(struct Expr * val1, struct Expr * val2);
struct Expr * apply(struct Expr * closure, struct ExprList * stack, struct BindListList * envList);
struct Expr * bind(struct Expr * name, struct Expr * value, struct BindListList * envList);
struct Expr * conditional(struct Expr * v1, struct Expr * v2, struct Expr * v3);
struct Expr * divide(struct Expr * num1, struct Expr * num2);
struct Expr * equal(struct Expr * num1, struct Expr * num2);
struct Expr * lessThan(struct Expr * num1, struct Expr * num2);
struct Expr * load(struct Expr * fileName, struct ExprList * stack, struct BindListList * envList);
struct Expr * mul(struct Expr * num1, struct Expr * num2);
struct Expr * negate(struct Expr * number);
struct Expr * not(struct Expr * val);
struct Expr * or(struct Expr * val1, struct Expr * val2);
struct Expr * rem(struct Expr * num2, struct Expr * num1);
struct Expr * sub(struct Expr * num1, struct Expr * num2);
void excStack(struct ExprList * stack);
void popStack(struct ExprList * stack);
void quit();

#endif

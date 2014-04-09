#ifndef BIND_H
#define BIND_H

#include "defs.h"

struct Bind {
  struct Expr * name;
  struct Expr * value;
};

struct Bind * Bind(struct Expr * n, struct Expr * v);
char * bind2string(struct Bind * t);

#endif

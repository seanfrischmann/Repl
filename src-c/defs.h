// see answer given by torek on May 30 '13 at 9:02 for explanation of why this needs to be done
// http://stackoverflow.com/questions/16831605/strange-compiler-warning-c-warning-struct-declared-inside-parameter-list
struct Expr;
struct ExprList;
struct Bind;
struct BindList;
struct BindListList;

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bind.h"
#include "BindList.h"
#include "BindListList.h"
#include "Expr.h"
#include "ExprList.h"
#include "Primitives.h"
#include "REPL.h"

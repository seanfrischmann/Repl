#ifndef EXPR_H
#define EXPR_H

#include "defs.h"

/*
  Definition of the Expr type, and related functions.
  Expr is a disjoint union type, with disjuncts defined by expr_type, stack_ops, etc.
 */

enum expr_type { NUMBER=1000, BOOLEAN, ERROR, STRING, NAME, PRIMITIVE, STACKOP, QUIT, CLOSURE };
enum stack_ops { POP=2000, EXC };
enum prim_ops { NEG=3000, ADD, SUB, MUL, DIV, REM, NOT, AND, OR, EQUAL, LESSTHAN, IF };
enum other_ops { BIND_OP=4000, LOAD_OP, APPLY_OP };
enum bool_values { FALSE=0, TRUE };

struct Closure {
  char * name;
  struct BindListList * environment;
  struct ExprList * body;
};

typedef struct Expr * (*primitiveCodePtr)();
struct Primitive {
  int arity;
  char * name;
  int type;
  primitiveCodePtr code;  //  struct Expr * (*code)();
};

typedef void (*stackOpCodePtr)();
struct StackOp {
  int arity;
  char * name;
  int type;
  void (*code)();
};

struct ApplyOp { char * name; };
struct BindOp  { char * name; };
struct Boolean { int value; };
//struct Closure -- ABOVE
struct Error   { char * name; };
struct Load    { char * name; };
struct Name    { char * name; };
struct Number  { int value; };
//struct Primitive -- ABOVE
struct Quit    { char * name; };
//struct StackOp -- ABOVE
struct String  { char * name; };

struct Expr {
  int type;
  union {
    struct ApplyOp apply;
    struct BindOp bind;
    struct Boolean bool;
    struct Closure closure;
    struct Error error;
    struct Load load;
    struct Name name;
    struct Number number;
    struct Primitive primitive;
    struct Quit quit;
    struct StackOp stack;
    struct String string;
  } subtype;
};

// type predicates
int isApplyOp(struct Expr * t);
int isBindOp(struct Expr * t);
int isBoolean(struct Expr * t);
int isClosure(struct Expr * t);
int isError(struct Expr * t);
int isLoad(struct Expr * t);
int isName(struct Expr * t);
int isNumber(struct Expr * t);
int isPrimitive(struct Expr * t);
int isQuit(struct Expr * t);
int isStackOp(struct Expr * t);
int isString(struct Expr * t);

// accessors
int valueOf(struct Expr * t);
char * nameOf(struct Expr * t);
int arityOf(struct Expr * t);
primitiveCodePtr codeOfPrimitive(struct Expr * t);
stackOpCodePtr codeOfStackOp(struct Expr * t);

// toString
char * expression2string(struct Expr * t);

// constructors
struct Expr * ApplyOp();
struct Expr * BindOp();
struct Expr * Boolean(int b);
struct Expr * Closure(char * bodyAsString, struct BindListList * envList);
struct Expr * Error();
struct Expr * Load();
struct Expr * Name(char * str);
struct Expr * Number(int num);
struct Expr * Primitive(int type, char * name, int arity, struct Expr * (*code)() );
struct Expr * Quit();
struct Expr * StackOp(int type, char * name, int arity, void (*code)() );
struct Expr * String(char * str);

#endif

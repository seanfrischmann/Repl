#include "Expr.h"

int isApplyOp(struct Expr * t)   { return (*t).type == APPLY_OP; }
int isBindOp(struct Expr * t)    { return (*t).type == BIND_OP; }
int isBoolean(struct Expr * t)   { return (*t).type == BOOLEAN; }
int isClosure(struct Expr * t)   { return (*t).type == CLOSURE; }
int isError(struct Expr * t)     { return (*t).type == ERROR; }
int isLoad(struct Expr * t)      { return (*t).type == LOAD_OP; }
int isName(struct Expr * t)      { return (*t).type == NAME; }
int isNumber(struct Expr * t)    { return (*t).type == NUMBER; }
int isPrimitive(struct Expr * t) { return (*t).type == PRIMITIVE; }
int isQuit(struct Expr * t)      { return (*t).type == QUIT; }
int isStackOp(struct Expr * t)   { return (*t).type == STACKOP; }
int isString(struct Expr * t)    { return (*t).type == STRING; }

int valueOf(struct Expr * t) {
  if (isBoolean(t)) { return (*t).subtype.bool.value;   }
  if (isNumber(t))  { return (*t).subtype.number.value; }
  return -1;
}

char * nameOf(struct Expr * t) {
  if (isApplyOp(t))   { return (*t).subtype.apply.name;     }
  if (isBindOp(t))    { return (*t).subtype.bind.name;      }
  if (isClosure(t))   { return (*t).subtype.closure.name;   }
  if (isError(t))     { return (*t).subtype.error.name;     }
  if (isLoad(t))      { return (*t).subtype.load.name;      }
  if (isName(t))      { return (*t).subtype.name.name;      }
  if (isPrimitive(t)) { return (*t).subtype.primitive.name; }
  if (isQuit(t))      { return (*t).subtype.quit.name;      }
  if (isStackOp(t))   { return (*t).subtype.stack.name;     }
  if (isString(t))    { return (*t).subtype.string.name;    }
  return NULL;
}

int arityOf(struct Expr * t) {
  if (isPrimitive(t)) { return (*t).subtype.primitive.arity; }
  if (isStackOp(t))   { return (*t).subtype.stack.arity;     }
  return -1;
}

//struct Expr * (*codeOf(struct Expr * t))() {
primitiveCodePtr codeOfPrimitive(struct Expr * t) {
  if (isPrimitive(t)) { return (*t).subtype.primitive.code; }
  return NULL;
}

stackOpCodePtr codeOfStackOp(struct Expr * t) {
  if (isStackOp(t))   { return (*t).subtype.stack.code;     }
  return NULL;
}

char * expression2string(struct Expr * t){
  char * str;
  switch( (*t).type ){
  case BOOLEAN:
    str = strdup(valueOf(t) == TRUE ? ":true:" : ":false:");
    break;
  case NUMBER: {
      int v = valueOf(t);
      int number_of_characters = v==0? 1 : (int) floor(log10(abs(v)));
      if (valueOf(t) < 0) { number_of_characters++; }
      str = (char *) malloc(sizeof(char)*number_of_characters);
      sprintf(str,"%d",v);
    }
    break;
  case STRING: {
      char * name = nameOf(t);
      str = (char *) malloc(sizeof(char)*(strlen(name)+3)); // +2 for ", +1 for \0
      str[0]='"';
      str[1]='\0';
      str = strcat(strcat(str,name),"\"");
    }
    break;
  default:
    str = strdup(nameOf(t));
  }   
  return str;
}

struct Expr * ApplyOp() {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));  
  (*x).type = APPLY_OP;
  (*x).subtype.quit.name = ":apply:";
  return x;
}

struct Expr * BindOp() {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));  
  (*x).type = BIND_OP;
  (*x).subtype.quit.name = ":bind:";
  return x;
}

struct Expr * Boolean(int b) {
    struct Expr * px = (struct Expr *) (malloc(sizeof(struct Expr)));;
    (*px).type = BOOLEAN;
    (*px).subtype.bool.value = b;
    return px;
}

struct Expr * Closure(char * bodyAsString, struct BindListList * envList) {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));;
  (*x).type = CLOSURE;
  (*x).subtype.closure.name = ":closure:";

  // STATIC LINK
  (*x).subtype.closure.environment = bindListList_Empty();
  (*x).subtype.closure.environment->cdr = envList->cdr;

  // PARSE BODY
  // (*x).subtype.bodyAsString = bodyAsString;

  struct ExprList *bodyAsTokenList = exprList_Empty();
  /* get the first token */
  char * string = strtok(bodyAsString," \n\t");

  /* walk through other tokens */
  while( string != NULL ) {
    struct Expr * token = parse(string,envList);
    exprList_push(bodyAsTokenList,token);
    string = strtok(NULL," \n\t");
  }
  (*x).subtype.closure.body = reverseCopy(bodyAsTokenList);
  return x;
}

struct Expr * Error() {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));;
  (*x).type = ERROR;
  (*x).subtype.error.name = ":error:";
  return x;
}

struct Expr * Load() {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));  
  (*x).type = LOAD_OP;
  (*x).subtype.quit.name = ":load:";
  return x;
}

struct Expr * Name(char * str) {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));;
  (*x).type = NAME;
  (*x).subtype.name.name = malloc(sizeof(char)*strlen(str));
  strcpy((*x).subtype.name.name,str);
  return x;
}

struct Expr * Number(int num) {
    struct Expr * px = (struct Expr *) (malloc(sizeof(struct Expr)));;
    (*px).type = NUMBER;
    (*px).subtype.number.value = num;
    return px;
}

struct Expr * Primitive(int type, char * name, int arity, struct Expr * (*code)() ) {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));  
  (*x).type = PRIMITIVE;
  (*x).subtype.primitive.type = type;
  (*x).subtype.primitive.arity = arity;
  (*x).subtype.primitive.name = malloc(sizeof(char)*strlen(name));
  strcpy((*x).subtype.primitive.name,name);
  (*x).subtype.primitive.code = code;
  return x;
}

struct Expr * Quit() {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));  
  (*x).type = QUIT;
  (*x).subtype.quit.name = ":quit:";
  return x;
}

struct Expr * StackOp(int type, char * name, int arity, void (*code)() ) {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));  
  (*x).type = STACKOP;
  (*x).subtype.stack.type = type;
  (*x).subtype.stack.arity = arity;
  (*x).subtype.stack.name = malloc(sizeof(char)*strlen(name));
  strcpy((*x).subtype.stack.name,name);
  (*x).subtype.stack.code = code;
  return x;
}

struct Expr * String(char * str) {
  struct Expr * x = (struct Expr *) (malloc(sizeof(struct Expr)));;
  (*x).type = STRING;
  (*x).subtype.string.name = malloc(sizeof(char)*strlen(str));
  strcpy((*x).subtype.string.name,str);
  return x;
}


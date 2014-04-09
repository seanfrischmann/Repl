#include "REPL.h"

static char * PROMPT_MAIN = "repl> ";
static char * PROMPT_MORE = "repl+ ";

int whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n';
}

/* Meta-langage stack operations */

/* Apply different types of operators */

// applies a ternary operator that takes its arguments from the stack
void applyTernary(struct Expr * (*f)(), struct ExprList * stack) {
  struct Expr * x = exprList_pop(stack); // boolean
  struct Expr * y = exprList_pop(stack); // else value
  struct Expr * z = exprList_pop(stack); // then value
  if (z != NULL && y != NULL && x != NULL) {
    struct Expr * result = (*f)(z,y,x);
    if (isError(result)) {
      exprList_push(stack,z);
      exprList_push(stack,y);
      exprList_push(stack,x);
    }
    exprList_push(stack,result);
  }
  else if (z == NULL && y == NULL && x == NULL) {
    // stack was empty
    exprList_push(stack,Error());
  }
  else if (z == NULL && y == NULL) {
    // stack had ONE item on it
    exprList_push(stack,x);
    exprList_push(stack,Error());
  }
  else { // (z == NULL) {
    // stack had TWO items on it
    exprList_push(stack,y);
    exprList_push(stack,x);
    exprList_push(stack,Error());
  }
}

// applies a binary operator that takes its arguments from the stack
void applyBinary(struct Expr * (*f)(), struct ExprList * stack) {
  struct Expr * x = exprList_pop(stack);
  struct Expr * y = exprList_pop(stack);
  if (y != NULL && x != NULL) {
    struct Expr * result = (*f)(y,x);
    if (isError(result)) {
      exprList_push(stack,y);
      exprList_push(stack,x);
    }
    exprList_push(stack,result);
  }
  else if (y == NULL && x == NULL) {
    // stack was empty
    exprList_push(stack,Error());
  }
  else { 
    // stack had ONE item on it
    exprList_push(stack,x);
    exprList_push(stack,Error());
  }
}

// applies a unary operator that takes its argument from the stack
void applyUnary(struct Expr * (*f)(), struct ExprList * stack) {
  struct Expr * x = exprList_pop(stack);
  if (x != NULL) {
    struct Expr * result = (*f)(x);
    if (isError(result)) {
      exprList_push(stack,x);
    }
    exprList_push(stack,result);
  }
  else { // stack was empty
    exprList_push(stack,Error());
  }
}

// applies an operator that manipulates the stack
void applyStackOperation(void (*f)(), struct ExprList * stack) {
  (*f)(stack);
}

// applyBind: bind takes three arguments - two from the stack, and a list of environments
void applyBind(struct ExprList * stack, struct BindListList * envList) {
  struct Expr * x = exprList_pop(stack);
  struct Expr * y = exprList_pop(stack);
  if (y != NULL && x != NULL) {
    struct Expr * result = bind(y,x,envList);
    if (isError(result)) {
      exprList_push(stack,y);
      exprList_push(stack,x);
    }
    exprList_push(stack,result);
  }
  else if (y == NULL && x == NULL) {
    // stack was empty
    exprList_push(stack,Error());
  }
  else { 
    // stack had ONE item on it
    exprList_push(stack,x);
    exprList_push(stack,Error());
  }
}

// applyApply: apply takes three arguments - one from the stack, the stack and a list of environments
void applyApply(struct ExprList * stack, struct BindListList * envList) {
  struct Expr * x = exprList_pop(stack);
  if (x != NULL) {
    struct Expr * result = apply(x,stack,envList);
    if (result != NULL) {  // result will be null if application is successful, error otherwise
      exprList_push(stack,x);
      exprList_push(stack,result);
    }
  }
  else if (x == NULL) { // stack was empty
    exprList_push(stack,Error());
  }
}

// applyLoad: load takes three arguments - one from the stack, the stack and a list of environments
void applyLoad(struct ExprList * stack, struct BindListList * envList) {
  struct Expr * x = exprList_pop(stack);
  if (x != NULL) {
    struct Expr * result = load(x,stack,envList);
    if (isError(result)) {
      exprList_push(stack,x);
    }
    exprList_push(stack,result);
  }
  else if (x == NULL) {
    // stack was empty
    exprList_push(stack,Error());
  }
}

// applyLookUp: bindListList_find takes two arguments - a name and a list of environments
void applyLookUp(struct Expr * name, struct ExprList * stack, struct BindListList * envList) {
  struct Expr * result = bindListList_find(name,envList);
  if (result == NULL) {
    exprList_push(stack,name);
  }
  else {
    exprList_push(stack,result);
  }
}

// determines whether a given string satisfies the definition of a string
int stringIsAString(char * str) {
  if (str[0] == '"' && str[strlen(str)-1] == '"') {
    for (int i=1; i<strlen(str)-2; i++) {
      if (str[i] == '"') {
	return 0;
      }
    }
    return 1;
  }
  return 0;
}

// determines whether a given string satisfies the definition of a closure
int stringIsAClosure(char * str) {
  return str[0] == '{' 
    && str[strlen(str)-1] == '}'
    && whitespace(str[1]) 
    && whitespace(str[strlen(str)-2])
  ;
}

// determines whether a given string satisfies the definition of a name
int stringIsAName(char * str) {
  if (strlen(str) > 0) {
    if (! isalpha(str[0]) ) {
      return 0;
    }
    for (int i=0; i<strlen(str); i++) {
      if (! isalnum(str[i])) {
	return 0;
      }
    }
    return 1;
  }
  return 0; // this return really should never happen - str should never be empty!
}

// removes first a last character from a string
// helpful to remove quotation marks from around a string
// helpful to remove brackes from around a closure
char * stripFirstAndLast(char * str) {
  char * noQuotes = (char *) malloc(sizeof(char)*(strlen(str)-2+1)); // -2 for ", +1 for \0
  for (int i=1; i<strlen(str)-1; i++) {
    noQuotes[i-1] = str[i];
  }
  noQuotes[strlen(str)-1] = '\0';
  return noQuotes;
}

/*
 * maps a string to an expression
 *
 * envList must be passed along in case we are creating a closure,
 * so the closure can capture the current environment
 */
struct Expr * parse(char * string, struct BindListList * envList){
       /* BOOLEAN LITERALS */
       if(strcmp(string,":true:")  == 0) { return Boolean(TRUE); }
  else if(strcmp(string,":false:") == 0) { return Boolean(FALSE); }
       /* ERROR LITERAL */
  else if(strcmp(string,":error:") == 0) { return Error(); }
       /* BOOLEAN OPERATORS */
  else if(strcmp(string,"not")== 0)      { return Primitive(NOT, "not", 1, not); }
  else if(strcmp(string,"or")== 0)       { return Primitive(OR,  "or" , 2, or); }
  else if(strcmp(string,"and")== 0)      { return Primitive(AND, "and", 2, and); }
       /* NUMERIC OPERATORS */
  else if(strcmp(string,"add")== 0)      { return Primitive(ADD, "add", 2, add); }
  else if(strcmp(string,"sub")== 0)      { return Primitive(SUB, "sub", 2, sub); }
  else if(strcmp(string,"mul")== 0)      { return Primitive(MUL, "mul", 2, mul); }
  else if(strcmp(string,"div")== 0)      { return Primitive(DIV, "divide", 2, divide); }
  else if(strcmp(string,"rem")== 0)      { return Primitive(REM, "rem", 2, rem); }
  else if(strcmp(string,"neg")== 0)      { return Primitive(NEG, "negate", 1, negate); }
       /* RELATIONAL OPERATORS */
  else if(strcmp(string,"equal")== 0)    { return Primitive(EQUAL, "equal", 2, equal); }
  else if(strcmp(string,"lessThan")== 0) { return Primitive(LESSTHAN, "lessThan", 2, lessThan); }
       /* APPLY OPERATOR */
  else if(strcmp(string,"apply")== 0)    { return ApplyOp(); }
       /* CONDITIONAL OPERATORS */
  else if(strcmp(string,"if")== 0)       { return Primitive(IF, "if", 3, conditional); }
       /* STACK OPERATORS */
  else if(strcmp(string,"pop")== 0)      { return StackOp(POP, "pop", 1, popStack); }
  else if(strcmp(string,"exc")== 0)      { return StackOp(EXC, "exc", 2, excStack); }
       /* BIND OPERATORS */
  else if(strcmp(string,"bind")== 0)     { return BindOp(); }
       /* LOAD */
  else if(strcmp(string,"load")== 0)     { return Load(); }
       /* QUIT */
  else if(strcmp(string,"quit")== 0)     { return Quit(); }
       /* NUMBERS */ 
  else if(strspn(string, "0123456789-") == strlen(string)) { return Number(atoi(string)); }
       /* STRING (could be multi-line) */
  else if(stringIsAString(string))       { return String(stripFirstAndLast(string)); }
       /* CLOSURE (could be multi-line) */
  else if(stringIsAClosure(string))      { return Closure(stripFirstAndLast(string),envList); }
       /* NAME */
  else if(stringIsAName(string))         { return Name(string); }
       /* UNKNOWN INPUT */
  else { return Error(); }
}

/* 
 * evaluates an expression (token) with respect to the current stack and list of environments
 */
struct ExprList * eval(struct Expr * token, struct ExprList * stack, struct BindListList * envList){
       if( isApplyOp(token) ) { applyApply(stack, envList); }
  else if( isBindOp(token) )  { applyBind(stack, envList); }
  else if( isBoolean(token) ) { exprList_push(stack, token); }
  else if( isClosure(token) ) { exprList_push(stack, token); }
  else if( isError(token) )   { exprList_push(stack, token); }
  else if( isError(token) )   { exprList_push(stack, token); }
  else if( isLoad(token) )    { applyLoad(stack, envList); }
  else if( isName(token) )    { applyLookUp(token, stack, envList); }
  else if( isNumber(token) )  { exprList_push(stack, token); }
  else if( isPrimitive(token) && arityOf(token) == 1 ) { applyUnary(codeOfPrimitive(token), stack); }
  else if( isPrimitive(token) && arityOf(token) == 2 ) { applyBinary(codeOfPrimitive(token), stack); }
  else if( isPrimitive(token) && arityOf(token) == 3 ) { applyTernary(codeOfPrimitive(token), stack); }
  else if( isQuit(token) )    { quit(); }
  else if( isStackOp(token) ) { applyStackOperation(codeOfStackOp(token), stack); }
  else if( isString(token) )  { exprList_push(stack, token); }
  else { exprList_push(stack,Error()); }
  return stack;
}

/*
 * Reads the next token from an input stream
 * interactive determines whether prompts should be printed
 */
char * nextToken(FILE * inStr, struct ExprList * stack, int interactive) {
  static int needToPrintPrompt = 1;
  if (interactive && needToPrintPrompt) {
    exprList_print(stack);
    printf("%s",PROMPT_MAIN);
    fflush(stdout);
    needToPrintPrompt = 0;
  }
  char * string = (char *) malloc(sizeof(char)*500);
  int i=0;
  string[i] = '\0';
  // Remove whitespace before token (first token after \n)
  char ch = getc(inStr);
  while (ch != EOF && whitespace(ch)) {
    ch = getc(inStr);
  }
  // Read token
  if (ch == '"') { // start of string literal
    string[i++] = ch;
    string[i] = '\0';
    ch = getc(inStr);
    while (ch != EOF && ch!='"') { // ignore whitespace in string
      string[i++] = ch;
      string[i] = '\0';
      if (interactive && ch == '\n') {
	printf("%s",PROMPT_MORE);  // multi-line string
	fflush(stdout);
      }
      ch = getc(inStr);
    }
    string[i++] = ch;
    string[i] = '\0';
    ch = getc(inStr);
    while (ch != EOF && !whitespace(ch)) { // now look for whitespace
      string[i++] = ch;
      string[i] = '\0';
      ch = getc(inStr);
    }
  }
  else if (ch == '{') { // start of closure
    string[i++] = ch;
    string[i] = '\0';
    ch = getc(inStr);
    while (ch != EOF && ch!='}') { // ignore whitespace in closure
      string[i++] = ch;
      string[i] = '\0';
      if (interactive && ch == '\n') {
	printf("%s",PROMPT_MORE);  // multi-line closure
	fflush(stdout);
      }
      ch = getc(inStr);
    }
    string[i++] = ch;
    string[i] = '\0';
    ch = getc(inStr);
    while (ch != EOF && !whitespace(ch)) { // now look for whitespace
      string[i++] = ch;
      string[i] = '\0';
      ch = getc(inStr);
    }
  }
  else {
    while (ch != EOF && !whitespace(ch)) {
      string[i++] = ch;
      string[i] = '\0';
      ch = getc(inStr);
    }
  }
  // Remove whitespace after token
  while (ch != EOF && ch != '\n' && whitespace(ch)) {
    ch = getc(inStr);
  }
  if (ch == '\n') {  // Print prompt if we found '\n'
    needToPrintPrompt = 1;
  }
  else { // otherwise we read too far - put ch back on stream
    ungetc(ch, inStr);
  }
  return string;
}

/* Main repl function */
void repl(){
  struct Expr *token;
  struct ExprList *stack = exprList_Empty();
  struct BindList *env = bindList_Empty();
  struct BindListList *envList = bindListList_Empty();
  bindListList_push(envList,env);

  while( 1 ) {
    token = parse(nextToken(stdin,stack,INTERACTIVE),envList);
    eval(token,stack,envList);
  }
}


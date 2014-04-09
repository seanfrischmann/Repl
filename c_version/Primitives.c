#include "Primitives.h"

/* IMPLEMENTATIONS OF OBJECT LANGUAGE PRIMITIVES */

// TERNARY FUNCTIONS (3 args from stack)

/*   IF: conditional operator [ v1 v2 v3 if ]  */
struct Expr * conditional(struct Expr * v1, struct Expr * v2, struct Expr * v3){
  if (isBoolean(v3)) {
    if (valueOf(v3) == TRUE) {
      return v1;
    }
    else {
      return v2;
    }
  }
  return Error();
}


// BINARY FUNCTIONS (2 args from stack)

/*   OR: logical or   */
struct Expr * or(struct Expr * val1, struct Expr * val2){
  if (isBoolean(val1) && isBoolean(val2)) {
    return Boolean(valueOf(val1) || valueOf(val2));
  }
  return Error();
}

/*   AND: logical and	 */
struct Expr * and(struct Expr * val1, struct Expr * val2){
  if (isBoolean(val1) && isBoolean(val2)) {
    return Boolean(valueOf(val1) && valueOf(val2));
  }
  return Error();
}


/*   EQUAL: relational function   */
struct Expr * equal(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2)) {
    if (valueOf(num1) == valueOf(num2)) {
      return Boolean(TRUE);
    }
    else {
      return Boolean(FALSE);
    }
  }
  return Error();
}

/*   LESSTHAN: relational function   */
struct Expr * lessThan(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2)) {
    if (valueOf(num1) < valueOf(num2)) {
      return Boolean(TRUE);
    }
    else {
      return Boolean(FALSE);
    }
  }
  return Error();
}

/*   ADD: Add function   */
struct Expr * add(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2)) {
    return Number(valueOf(num1) + valueOf(num2));
  }
  return Error();
}

/*   SUB: Subtract function	 */
struct Expr * sub(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2)) {
    return Number(valueOf(num1) - valueOf(num2));
  }
  return Error();
}


/*  MUL: Multiply function	*/
struct Expr * mul(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2)) {
    return Number(valueOf(num1) * valueOf(num2));
  }
  return Error();
}

/*   DIV: Divide function   */
int c_divide(int a2, int a1); // forward declaration
struct Expr * divide(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2) && (valueOf(num2) != 0)) {
    return Number( c_divide(valueOf(num1), valueOf(num2)) );
  }
  return Error();
}

/*   REM: Remainder function	  */
struct Expr * rem(struct Expr * num1, struct Expr * num2){
  if (isNumber(num1) && isNumber(num2) && (valueOf(num2) != 0)) {
    return Number( valueOf(num1) - valueOf(num2) * valueOf(divide(num1,num2)) );
  }
  return Error();
}


// UNARY FUNCTIONS (1 arg from stack)

/*   NOT: logical negation   */
struct Expr * not(struct Expr * val){
  if (isBoolean(val)) { return Boolean(!valueOf(val)); }
  return Error();
}

/*   NEG: negate value   */
struct Expr * negate(struct Expr * number){
  if (isNumber(number)) { return Number(-valueOf(number)); }
  return Error();
}

// STACK OPERATIONS (arg is stack)

/*   POP: Pop top value from stack	 */
void popStack(struct ExprList *stack){
  struct Expr * t;
  if((t = exprList_pop(stack)) == NULL){
    exprList_push(stack,Error());
  }
}

/*   EXC: Swap top 2 values of stack	  */   
void excStack(struct ExprList *stack){
  struct Expr * x = exprList_pop(stack);
  struct Expr * y = exprList_pop(stack);
  //make sure there are 2 values to exchange
  if(x == NULL){ //no values on the stack
    exprList_push(stack,Error());
  }
  else if(y == NULL){ //only 1 value was on the stack
  	exprList_push(stack,x);
  	exprList_push(stack,Error());
  }
  else{
  	exprList_push(stack, x);
  	exprList_push(stack, y);
  }
}

// OTHER FUNCTIONS

/*   QUIT: Exit the interpreter normally */
void quit() {
  exit(EXIT_SUCCESS);
}

/*   BIND: adds a binding to the binding list   */
struct Expr * bind(struct Expr * name, struct Expr * value, struct BindListList * envList){
  if (isName(name)) {
    bindList_push(bindListList_top(envList), Bind(name, value));
    return value;
  }
  return Error();
}


/*   APPLY: applies a closure */
struct Expr * apply(struct Expr * closure, struct ExprList * stack, struct BindListList * envList){
  if (isClosure(closure)) {
    struct BindListList * localList = bindListList_Empty();  // create new environment list for apply
    localList->cdr = closure->subtype.closure.environment->cdr;  // link it to closure's env list
    struct BindList * local = bindList_Empty(); // create new local environment
    bindListList_push(localList,local);  // push it onto local list of environments

    struct ExprList * top = (*closure).subtype.closure.body->cdr; // the body of the closure
    while (top != NULL) {
      eval(top->car, stack, localList);  // evaluate token from body with respect to stack and localList
      top = top->cdr;
    }

    return NULL;
  }
  return Error();
}

/*   LOAD: Load contents of a file */
struct Expr * load(struct Expr * fileName, struct ExprList * stack, struct BindListList * envList) {
  if (isString(fileName)) {
    FILE * stream = fopen(nameOf(fileName),"r"); // try to create stream connected to file
    if (stream == NULL) {
      return Boolean(FALSE);  // didn't work - push :false: to stack [NB: HW4 style load]
    }
    char * inputString = nextToken(stream,stack,NON_INTERACTIVE);  // use nextToken non-interactively
    while( strcmp(inputString,"")!=0 ) {
      struct Expr * token = parse(inputString, envList);
      stack = eval(token,stack,envList);
      inputString = nextToken(stream,stack,NON_INTERACTIVE);
    }
    fclose(stream);  // close stream
    return Boolean(TRUE);  // all went OK
  }
  return Error(); // argument to load was not a string
}


/*   helper function for DIV   */
int c_divide(int a2, int a1) { // computing a2 a1 div == a2/a1
  if (a2<0) {
    if ((a2/a1)*a1 > a2) { return a2/a1 + (a1 < 0 ? 1 : -1); }
                    else { return a2/a1; }
  }
  else { return a2/a1; }
}


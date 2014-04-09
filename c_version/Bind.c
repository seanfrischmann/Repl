#include "Bind.h"

struct Bind * Bind(struct Expr * n, struct Expr * v) {
  struct Bind * px = (struct Bind *) (malloc(sizeof(struct Bind)));;
  (*px).name = n;
  (*px).value = v;
  return px;
}

char * bind2string(struct Bind * t){
  char * name  = expression2string((*t).name);
  char * value = expression2string((*t).value);
  char * str = malloc(sizeof(char)*(1+strlen(name)+1+strlen(value)+2));
  str[0]='(';
  str[1]='\0';
  str = strcat(strcat(strcat(strcat(str,name),","),value),")");
  return str;
}


objects = Bind.o BindList.o BindListList.o \
	Expr.o ExprList.o \
	Primitives.o \
	REPL.o \
	hw3.o

CC = gcc
FLAGS = -c -Wall -std=gnu99

hw3 : $(objects)
	$(CC) -Wall -lm -o hw3 $(objects) 

hw3.o : hw3.c
	$(CC) $(FLAGS) hw3.c

Bind.o : defs.h
	$(CC) $(FLAGS) Bind.c

BindList.o : defs.h
	$(CC) $(FLAGS) BindList.c

BindListList.o : defs.h
	$(CC) $(FLAGS) BindListList.c

Expr.o : defs.h
	$(CC) $(FLAGS) Expr.c

ExprList.o : defs.h
	$(CC) $(FLAGS) ExprList.c

Primitives.o : defs.h
	$(CC) $(FLAGS) Primitives.c

REPL.o : defs.h
	$(CC) $(FLAGS) REPL.c

clean :
	rm -fv hw3 $(objects) *~

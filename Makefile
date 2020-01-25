CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic
OBJ = LinkedList.o FileInput.o effects.o TurtleGraphics.o
LIB = -lm
EXEC = TurtleGraphics

$(EXEC) : rmTurtle $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(EXEC)

TurtleGraphicsSimple : $(OBJ) rmTurtle
	$(CC) -c TurtleGraphics.c $(CFLAGS) -DSIMPLE=1
	$(CC) $(OBJ) $(LIB) -o TurtleGraphicsSimple

TurtleGraphicsDebug : $(OBJ) rmTurtle
	$(CC) -c TurtleGraphics.c $(CFLAGS) -DDEBUG=1
	$(CC) $(OBJ) $(LIB) -o TurtleGraphicsDebug

TurtleGraphicsVerbose : $(OBJ) rmTurtle
	$(CC) -c TurtleGraphics.c $(CFLAGS) -DVERBOSE=1
	$(CC) $(OBJ) $(LIB) -o TurtleGraphicsVerbose

TurtleGraphicsMegaDebug : CFLAGS +=  -g
TurtleGraphicsMegaDebug : $(OBJ) rmTurtle
	$(CC) -c TurtleGraphics.c $(CFLAGS) -DDEBUG=1 -DVERBOSE=1
	$(CC) $(OBJ) $(LIB) -o TurtleGraphicsMegaDebug

LinkedList.o : LinkedList.c LinkedList.h
	$(CC) -c LinkedList.c $(CFLAGS)

FileInput.o : FileInput.c FileInput.h LinkedList.h
	$(CC) -c FileInput.c $(CFLAGS)

effects.o : effects.c effects.h
	$(CC) -c effects.c $(CFLAGS)

TurtleGraphics.o : TurtleGraphics.c TurtleGraphics.h LinkedList.h FileInput.h\
 effects.h
	$(CC) -c TurtleGraphics.c $(CFLAGS)

OptimiserFile.o : OptimiseFile.c LinkedList.h FileInput.h
	$(CC) -c OptimiseFile.c $(CFLAGS)

Optimiser : OptimiseFile.o LinkedList.o FileInput.o
	$(CC) OptimiseFile.o LinkedList.o FileInput.o -o Optimiser

clean :
	rm -f $(OBJ) $(EXEC) TurtleGraphicsSimple TurtleGraphicsDebug\
 TurtleGraphicsVerbose TurtleGraphicsMegaDebug OptimiseFile.o Optimiser

rmTurtle :
	rm -f TurtleGraphics.o

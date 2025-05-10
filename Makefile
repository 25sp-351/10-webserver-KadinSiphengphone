all: main

OBJS = main.o request.o header.o connection_handler.o calc.o response.o
CC = gcc
CFLAGS = -Wall

main: $(OBJS)
	gcc -o main $(CFLAGS) $(OBJS)

main.o: main.c

header.o: header.h header.c

request.o: request.h request.c

connection_handler.o: connection_handler.h connection_handler.c

calc.o: calc.h calc.c

response.o: response.h response.c

clean: 
	rm -f main $(OBJS)
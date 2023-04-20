CC = gcc
CFLAGS = -Wall 

all:
	$(CC) $(CFLAGS) -o ex1 zad1.c 
	$(CC) -o main_cpy main_cpy.c 
test: all
	./ex1 ignore
	./ex1 handler
	./ex1 mask
	./ex1 pending

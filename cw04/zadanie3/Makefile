CC = gcc
CFLAGS = -Wall
all:
	$(CC) $(CFLAGS) -o catch catcher.c 
	$(CC) $(CFLAGS) -o send sender.c 

test: all
	./catch
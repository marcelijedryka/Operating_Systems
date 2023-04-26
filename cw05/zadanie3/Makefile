CC = gcc
CFLAGS = -Wall
all:
	$(CC) $(CFLAGS) -o ex3 main.c 
	$(CC) $(CFLAGS) -o int integral.c 

test: all
	echo "Testing performance for DX = 0.000000001 and N = 1\n" >> raport.txt
	./ex3 0.000000001 1 >> raport.txt
	echo "\n" >> raport.txt
	echo "Testing performance for DX = 0.000000001 and N = 2\n" >> raport.txt
	./ex3 0.000000001 2 >> raport.txt
	echo "\n" >> raport.txt
	echo "Testing performance for DX = 0.000000001 and N = 5\n" >> raport.txt
	./ex3 0.000000001 5 >> raport.txt
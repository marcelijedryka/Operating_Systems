create_static: mylib.c 
	gcc -c mylib.c
	ar rcs libmylibstatic.a mylib.o

create_shared: mylib.c
	gcc -fPIC -c mylib.c
	gcc -shared mylib.o -o libmylibshared.so

clean_all:
	rm *.o *.a *.so




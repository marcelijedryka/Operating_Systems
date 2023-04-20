compile_static: mylib.c main.c
	gcc -c mylib.c
	ar rcs libmylibstatic.a mylib.o
	gcc -c main.c
	gcc main.o libmylibstatic.a -o main_static
	

compile_shared: mylib.c main.c
	gcc -fPIC -c mylib.c
	gcc -shared mylib.o -o libmylibshared.so
	gcc -c main.c
	gcc main.o -L. -Wl,-rpath=. -lmylibshared -o main_shared

compile_dynamic:
	gcc -fPIC -c mylib.c
	gcc -shared mylib.o -o libmylibshared.so
	gcc main_dynamic.c -o main_dynamic -ldl

compile_all:
	make compile_static compile_shared compile_dynamic

clean_all:
	rm *.o *.a *.so main_shared main_dynamic main_static
	
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


run_tests: 
	./main_static < test.txt > results_static.txt
	./main_shared < test.txt > results_shared.txt
	./main_dynamic < test.txt > results_dynamic.txt

compile_all:
	make compile_dynamic compile_shared compile_static


clean_all:
	rm  *.o *.so *.a results_static.txt results_dynamic.txt results_shared.txt main_static main_shared main_dynamic





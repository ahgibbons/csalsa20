all: salsafuncs.o salsa20.c
	gcc -O2 -o salsa.out salsa20.c salsafuncs.o

salsafuncs.o: salsafuncs.c
	gcc -O2 -c -o salsafuncs.o salsafuncs.c


clean:
	rm *.out *.o
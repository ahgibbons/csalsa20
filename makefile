all:
	gcc -O2 -o salsa.out salsa20.c

clean:
	rm *.out
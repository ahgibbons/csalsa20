# csalsa20
Salsa20 implementated in C
by Andrew Gibbons

Not for real world use. Made as a learning exercise.


## Install Instructions

Compile with `make all`.

But the code is simple enough to compile directly with
`gcc -O2 -o salsa20 salsa20.c salsafuncs.c`

## Run Instructions

Run with `./salsa20 inputfile keyfile noncefile outfile`

`inputfile` is a binary file of contents to be encrypted. In principle it can be any size.
`outfile` is a the destination for the encrypted contents. In princple it can be any size.
`keyfile` and `noncefile` are files containing at least 32 bytes. The first 32 bytes of the file are used as the key or nonce.

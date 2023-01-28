CS=src/tokenizer.c src/parser.c src/eval.c src/env.c src/gc.c src/utils.c src/std_procs.c src/debug_utils.c
TS=tests/tokenizer_tests.h tests/parser_tests.h
CFLAGS=-Wall -ggdb
CC=gcc

repl: src/main.c 
	$(CC) $(CFLAGS) -o repl src/main.c $(CS)

check: tests/main.c 
	$(CC) $(CFLAGS) -o test tests/main.c $(CS) $(TS)
	./test

clean:
	rm repl test *.a *.o


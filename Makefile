CS=src/tokenizer.c src/parser.c src/eval.c src/env.c src/gc.c src/utils.c src/std_procs.c src/debug_utils.c
CFLAGS=-Wall -ggdb
CC=gcc

repl: src/main.c $(CS)
	$(CC) $(CFLAGS) -o repl src/main.c $(CS)

check: tests/main.c 
	$(CC) $(CFLAGS) -o test tests/main.c $(CS)
	./test

clean:
	rm repl test *.a *.o


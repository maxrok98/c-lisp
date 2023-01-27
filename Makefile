CS=src/tokenizer.c src/parser.c src/eval.c src/env.c src/gc.c src/utils.c src/std_procs.c src/debug_utils.c
OBJS=tokenizer.o parser.o eval.o env.o gc.o utils.o std_procs.o debug_utils.o
CFLAGS=-Wall -lcunit -ggdb
CC=gcc

repl: src/main.c 
	$(CC) $(CFLAGS) -o repl src/main.c $(CS)

check: tests/main.c 
	$(CC) $(CFLAGS) -Isrc -o test tests/main.c $(CS) -lcunit
	./test

$(OBJS): $(CS)
	$(CC) $(CFLAGS) -c $(CS)


clean:
	rm repl test *.a *.o


CS=src/main.c src/tokenizer.c src/parser.c src/eval.c src/env.c src/gc.c src/utils.c src/std_procs.c src/debug_utils.c
OBJS=main.o tokenizer.o parser.o eval.o env.o gc.o utils.o std_procs.o debug_utils.o
CFLAGS=-Wall -ggdb
CC=gcc

repl:  
	$(CC) $(CFLAGS) -o repl $(CS)

$(OBJS): $(CS)
	$(CC) $(CFLAGS) -c $(CS)

clean:
	rm repl


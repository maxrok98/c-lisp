[![build](https://github.com/maxrok98/c-lisp/actions/workflows/build.yml/badge.svg)](https://github.com/maxrok98/c-list/actions/workflows/build.yml)
[![tests](https://github.com/maxrok98/c-lisp/actions/workflows/tests.yml/badge.svg)](https://github.com/maxrok98/c-list/actions/workflows/tests.yml)

# c-lisp
 My attempt on writing LISP interpreter in C.

## Todo list
- [x] Tokenizer
- [x] Parser
- [x] Calculator
- [x] Environment
- [x] Lambdas
- [x] Garbage Collector 
- [x] Conditions
- [x] Lists
- [x] Mutators
- [x] Read from file
- [x] Error handling

## Build
```
$ make
$ ./repl
$ make check
$ ./test
```

## Examples

### Calculator
Interpreter supports basic arithmetic operations.
```
> (+ 1 (- 2 3) (* 4 5) (/ 6 2))
23
```

### Definition
Functions are define by assigning lambda to variable
```
> (define a (+ 1 2))
> a
3
> (define square (lambda (n) (* n n)))
> (square 3)
9

```

### Conditions
```
> (if (< 2 4) (+ 41 1) (+ 68 1))
42
```

### Closures
Closures are procedures executed in environment they were defined.
```
> (define counter
    (lambda (n)
		    (lambda () (begin (set! n (+ 1 n)) n))))
> (define c1 (counter 10))
> (define c2 (counter 20))
> (c1)
11
> (c2)
21
```

### Lists
Lists are sequences of pairs (conses).
```
> (cons 1 2)
(1 . 2)
> (define l (cons 1 (cons 2 (cons 3 null))))
> l
(1 2 3)
> (car (cdr l))
2
> (set-car! l 4)
> l
(4 2 3)
```

### Load file
Interpreter may be opened with preloaded files.
```
$ ./repl -f examples/lists.scm
Scheme interpreter:
> (map (lambda (n) (* n n)) (list 1 2 3 4))
(1 4 9 16)
```


## Garbage Collector
 Garbage collection is implemented in a tracing way (specifically with a [mark-and-sweep](https://en.wikipedia.org/wiki/Tracing_garbage_collection#Na%C3%AFve_mark-and-sweep) algorithm), when value or environment created its reference is registered in GC pool.
After each REPL GC traces each accessible value and environment from a root enironment, marks them. Then it goes throug each reference in a list, and if it is marked its reference is copied to new GC pool, else it is freed.

## Error Handling
Interpreter supports several types of error handling.
```
> (+ b 4)
ERROR: b variable not bound
> ((+ 2 3) (- 2 3))
ERROR: integer is not a procedure
> ((lambda (a b) (+ a b)) 5)
ERROR: procedure expects 2 arguments, not 1
```

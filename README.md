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
- [ ] Lists
- [ ] Mutators
- [ ] Read from file

## Garbage Collector
 Garbage collection is implemented in a tracing way (specifically with a [mark-and-sweep](https://en.wikipedia.org/wiki/Tracing_garbage_collection#Na%C3%AFve_mark-and-sweep) algorithm), when value or environment created its reference is registered in GC pool.
After each REPL GC traces each accessible value and environment from a root enironment, marks them. Then it goes throug each reference in a list, and if it is marked its reference is copied to new GC pool, else it is freed.

#pragma once

#include <stdio.h> // only for snprintf
#include <stdbool.h>

#include "parser.h"

#define EVAL(lval, eval, finalize)\
	lval = eval;\
	if(lval->type == V_ERR) { finalize; return lval; }

#define ERR_BUFFER_SIZE 512

#define ERROR(...)\
	Lval* lval = createLval();\
	lval->type = V_ERR;\
	lval->value.errorMessage = (char*)malloc(sizeof(char) * ERR_BUFFER_SIZE);\
	snprintf(lval->value.errorMessage, ERR_BUFFER_SIZE, ##__VA_ARGS__);\
	return lval;

typedef enum LvalType {
	V_INTEGER,
	V_REAL,
	V_BOOLEAN,
	V_STRING,
	V_LAMBDA,
	V_SYMBOL,
	V_PAIR,
	V_NULL,
	V_ERR
} LvalType;

typedef struct Lambda Lambda;
typedef struct Pair Pair;

typedef struct Lval {
	LvalType type;

	union {
		int integer;
		double real;
		bool boolean;
		char* string;
		char* errorMessage;
		Pair* pair;
		Lambda* lambda;
	} value;

	bool gcMark;
} Lval;

#include "env.h"

struct Pair {
	Lval* car;
	Lval* cdr;
};

typedef enum LambdaType {
	NATIVE,
	CONSTRUCTED
} LambdaType;

struct Lambda {
		LambdaType type;
		Lval* (*function)(Lval**, int);
		int argc;
		char** argv;
		Env* env;
		Ast* body;
};

Lval* eval(Ast* ast, Env* env);
Lval* apply(Lval* operation, Lval** lvalList, int quantity, Env* env);
Lval* createLval(); // register referense in GC pool
void freeLval(Lval* lval);

#pragma once

#include <stdbool.h>

#include "parser.h"

typedef enum LvalType {
	V_INTEGER,
	V_REAL,
	V_BOOLEAN,
	V_STRING,
	V_LAMBDA,
	V_SYMBOL,
	V_NULL
} LvalType;

typedef struct Lambda Lambda;

typedef struct Lval {
	LvalType type;

	union {
		int integer;
		double real;
		bool boolean;
		char* string;
		Lambda* lambda;
	} value;

	bool gcMark;
} Lval;

#include "env.h"

typedef enum LambdaType {
	NATIVE,
	CONSTRUCTED
} LambdaType;

struct Lambda {
		LambdaType type;
		Lval* (*function)(Lval**, int, Env*);
		int argc;
		char** argv;
		Env* env;
		Ast* body;
};

Lval* eval(Ast* ast, Env* env);
Lval* apply(Lval* operation, Lval** lval, int quantity, Env* env);
Lval* createLval(); // register referense in GC pool
void freeLval(Lval* lval);

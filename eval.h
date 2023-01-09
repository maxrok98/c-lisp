#pragma once

#include <stdbool.h>

#include "parser.h"

typedef enum LvalType {
	V_INTEGER,
	V_REAL,
	V_BOOL,
	V_STRING,
	V_LAMBDA,
	V_SYMBOL,
	V_NULL
} LvalType;

typedef struct Lval {
	LvalType type;

	union {
		int integer;
		double real;
		bool boolean;
		char* string;

		// lambda parts
		void* env;
		Ast* body;
	} value;
} Lval;

#include "env.h"

Lval eval(Ast* ast, Env* env);
Lval apply(char operation, Lval* lval, int quantity, Env* env);

#pragma once

#include <stdbool.h>

#include "tokenizer.h"

typedef enum AstType {
	ATOM,
	LIST
} AstType;

typedef struct Ast {
	AstType type;
	void* value;
} Ast;

typedef enum AtomType {
	A_INTEGER,
	A_REAL,
	A_BOOLEAN,
	A_SYMBOL
} AtomType;

typedef struct Atom {
	AtomType type;

	union {
		long integer;
		double real;
		bool boolean;
		char* symbol;
	} value;
} Atom;

/*
 * List is implemented as dynamic array
 */
typedef struct List {
	Ast** astChildren;
	int quantity;
	
	int arraySize;
} List;

Ast* parse(Tokenizer* tokenizer);
List* initList();
void addAst(List* list, Ast* ast);
Ast* copyAst(Ast* ast);
void freeAst(Ast* ast);


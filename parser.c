#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"

#define INIT_LIST_SIZE 4

Ast* parse(Tokenizer* tokenizer) {
	Token* token = nextToken(tokenizer);
	if(token->tokenType == R_PAREN) {
		freeToken(token);
		return NULL;
	}

	Ast* ast = (Ast*)malloc(sizeof(Ast));
	if(token->tokenType != L_PAREN && token->tokenType != R_PAREN) {
		ast->type = ATOM;
		Atom* atom = (Atom*)malloc(sizeof(Atom));

		if(token->tokenType == INTEGER) {
			atom->type = A_INTEGER;
			atom->value.integer = atoi(token->value);
		}
		else if(token->tokenType == SYMBOL) {
			atom->type = A_SYMBOL;
			atom->value.symbol = (char*)malloc(sizeof(char) * strlen(token->value));
			strcpy(atom->value.symbol, token->value);
		}
		ast->value = (void*)atom;
	}
	else if(token->tokenType == L_PAREN) {
		ast->type = LIST;
	
		List* list = initList();
		Ast* childAst;
		while(childAst = parse(tokenizer)) {
			addAst(list, childAst);
		}
		ast->value = (void*)list;
	}
	freeToken(token);
	return ast;
}

List* initList() {
	List* list = (List*)malloc(sizeof(List));
	list->arraySize = INIT_LIST_SIZE;
	list->quantity = 0;
	
	list->astChildren = (Ast**)malloc(sizeof(Ast*) * list->arraySize);
	return list;
}

void addAst(List* list, Ast* ast) {
	if(list->quantity == list->arraySize) {
		list->arraySize += list->arraySize / 2;
		list->astChildren = (Ast**) realloc(list->astChildren, sizeof(Ast*) * list->arraySize);
	}
	list->astChildren[list->quantity] = ast;
	list->quantity++;
}

void freeAst(Ast* ast) {}

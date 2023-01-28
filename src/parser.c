#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"

#define INIT_LIST_SIZE 4

Ast* parse(Tokenizer* tokenizer) {
	Token* token = nextToken(tokenizer);
	if(token == NULL) {
		return NULL;
	}
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
		else if(token->tokenType == BOOLEAN) {
			atom->type = A_BOOLEAN;
			if(strcmp(token->value, "#t") == 0) {
				atom->value.boolean = true;
			}
			else {
				atom->value.boolean = false;
			}
		}
		else if(token->tokenType == SYMBOL) {
			atom->type = A_SYMBOL;
			atom->value.symbol = (char*)calloc(strlen(token->value) + 1, sizeof(char));
			strcpy(atom->value.symbol, token->value);
		}
		ast->value = (void*)atom;
	}
	else if(token->tokenType == L_PAREN) {
		ast->type = LIST;
	
		List* list = initList();
		Ast* childAst;
		while((childAst = parse(tokenizer))) {
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

Ast* copyAst(Ast* ast) {
	Ast* astCopy = (Ast*)malloc(sizeof(Ast));
	memcpy(astCopy, ast, sizeof(Ast));
	if(ast->type == ATOM) {
		Atom* atom = (Atom*)ast->value;
		astCopy->value = malloc(sizeof(Atom));

		Atom* atomCopy = (Atom*)astCopy->value;
		memcpy(atomCopy, atom, sizeof(Atom));

		if(atom->type == A_SYMBOL && atom->value.symbol != NULL) {
			atomCopy->value.symbol = (char*)calloc(strlen(atom->value.symbol) + 1, sizeof(char));
			strcpy(atomCopy->value.symbol, atom->value.symbol);
		}
	}
	else if(ast->type == LIST) {
		List* list = (List*)ast->value;
		astCopy->value = initList();

		List* listCopy = (List*)astCopy->value;

		for(int i = 0; i < list->quantity; i++) {
			addAst(listCopy, copyAst(list->astChildren[i]));
		}
	}
	return astCopy;
}

void freeAst(Ast* ast) {
	if(ast->type == ATOM) {
		Atom* atom = (Atom*)ast->value;
		if(atom->type == A_SYMBOL && atom->value.symbol != NULL) {
			free(atom->value.symbol);
		}
		free(atom);
	}
	else if(ast->type == LIST) {
		List* list = (List*)ast->value;
		for(int i = 0; i < list->quantity; i++) {
			freeAst(list->astChildren[i]);
		}
		free(list->astChildren);
		free(list);
	}
	free(ast);
}

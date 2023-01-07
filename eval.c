#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "parser.h"

Lval eval(Ast* ast) {
	Lval lval;
	if(ast->type == ATOM) {
		Atom* atom = (Atom*)ast->value;
		switch(atom->type) {
			case A_INTEGER:
				lval.type = V_INTEGER;
				lval.value.integer = atom->value.integer;
				break;
			default:
				printf("Unexpected atom!");
				exit(1);
		}
	}
	else if(ast->type == LIST) {
		List* list = (List*)ast->value;
		Lval* lvalList = (Lval*)malloc(sizeof(Lval) * list->quantity - 1);
		
		for(int i = 0; i < list->quantity - 1; i++) {
			lvalList[i] = eval(list->astChildren[i+1]);
		}
		Ast* firstChild = list->astChildren[0];
		lval = apply(((Atom*)firstChild->value)->value.symbol[0], lvalList, list->quantity - 1);
	}
	return lval;
}

Lval apply(char operation, Lval* lval, int quantity) {
	Lval lvalResult;
	lvalResult.type = V_INTEGER;
	lvalResult.value.integer = lval[0].value.integer;
	if(operation == '+') {
		for(int i = 1; i < quantity; i++)
			lvalResult.value.integer += lval[i].value.integer;
	}
	if(operation == '-') {
		for(int i = 1; i < quantity; i++)
			lvalResult.value.integer -= lval[i].value.integer;
	}
	if(operation == '*') {
		for(int i = 1; i < quantity; i++)
			lvalResult.value.integer *= lval[i].value.integer;
	}
	if(operation == '/') {
		for(int i = 1; i < quantity; i++)
			lvalResult.value.integer /= lval[i].value.integer;
	}
	return lvalResult;
}

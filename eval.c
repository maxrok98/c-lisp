#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#include "env.h"
#include "eval.h"
#include "parser.h"

Lval eval(Ast* ast, Env* env) {
	Lval lval;
	if(ast->type == ATOM) {
		Atom* atom = (Atom*)ast->value;
		switch(atom->type) {
			case A_INTEGER:
				lval.type = V_INTEGER;
				lval.value.integer = atom->value.integer;
				break;
			case A_SYMBOL:
				lval = getVar(atom->value.symbol, env);
				break;
			default:
				printf("Unexpected atom!");
				exit(1);
		}
	}
	else if(ast->type == LIST) {
		List* list = (List*)ast->value;

		// Special forms
		Ast* firstChild = list->astChildren[0];
		if(firstChild->type == ATOM) {
			Atom* atom = (Atom*)firstChild->value;
			if(atom->type == A_SYMBOL) {
				if(strcmp(atom->value.symbol, "define") == 0) {
					assert(list->quantity == 3);
					assert(((Atom*)list->astChildren[1]->value)->type == A_SYMBOL);

					Lval value = eval(list->astChildren[2], env);
					Atom* secondAtom = (Atom*)list->astChildren[1]->value;
					char* variable = secondAtom->value.symbol;

					addVar(variable, &value, env);

					Lval result;
					result.type = V_SYMBOL;
					result.value.string = variable;
					return result;
				}
			}
			
		}

		Lval* lvalList = (Lval*)malloc(sizeof(Lval) * list->quantity - 1);
		
		for(int i = 0; i < list->quantity - 1; i++) {
			lvalList[i] = eval(list->astChildren[i+1], env);
		}
		// TODO: Extend env
		lval = apply(((Atom*)firstChild->value)->value.symbol[0], lvalList, list->quantity - 1, env);
	}
	return lval;
}

Lval apply(char operation, Lval* lval, int quantity, Env* env) {
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

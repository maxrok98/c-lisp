#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#include "env.h"
#include "eval.h"
#include "parser.h"
#include "gc.h"

Lval* eval(Ast* ast, Env* env) {
	Lval* lval;

	if(ast->type == ATOM) {
		Atom* atom = (Atom*)ast->value;
		switch(atom->type) {
			case A_INTEGER:
				lval = createLval();
				lval->type = V_INTEGER;
				lval->value.integer = atom->value.integer;
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

					Lval* value = eval(list->astChildren[2], env);
					Atom* secondAtom = (Atom*)list->astChildren[1]->value;
					char* variable = secondAtom->value.symbol;

					addVar(variable, value, env);

					Lval* result = createLval();
					result->type = V_SYMBOL;
					result->value.string = (char*)malloc(sizeof(char) * strlen(variable) + 1);
					strcpy(result->value.string, variable);
					return result;
				}
				if(strcmp(atom->value.symbol, "lambda") == 0) {
					assert(list->quantity == 3);
					assert(list->astChildren[1]->type == LIST);
					assert(list->astChildren[2]->type == LIST);

					Lval* lval = createLval();
					lval->type = V_LAMBDA;
					Lambda* lambda = (Lambda*)malloc(sizeof(Lambda));
					lval->value.lambda = lambda;

					lambda->type = CONSTRUCTED;
					lambda->argc = ((List*)list->astChildren[1]->value)->quantity;
					lambda->argv = (char**)malloc(sizeof(char*) * lambda->argc);
					for(int i = 0; i < lambda->argc; i++) {
 						Ast* astChild = ((List*)list->astChildren[1]->value)->astChildren[i];

						assert(astChild->type == ATOM);
						assert(((Atom*)astChild->value)->type == A_SYMBOL);

						char* arg = ((Atom*)astChild->value)->value.symbol;
						lambda->argv[i] = (char*)malloc(sizeof(char) * strlen(arg) + 1);
						strcpy(lambda->argv[i], arg);
					}
					lambda->env = env;
					lambda->body = copyAst(list->astChildren[2]);
	
					return lval;
				}
			}
			
		}

		Lval** lvalList = (Lval**)malloc(sizeof(Lval*) * list->quantity - 1);
		
		for(int i = 1; i < list->quantity; i++) {
			lvalList[i-1] = eval(list->astChildren[i], env);
		}
		Lval* operation = eval(firstChild, env);
		lval = apply(operation, lvalList, list->quantity - 1, env);
		free(lvalList);
	}
	return lval;
}

Lval* apply(Lval* operation, Lval** lval, int quantity, Env* env) {
	assert(operation->type == V_LAMBDA);
	Lambda* lambda = operation->value.lambda;
	if(lambda->type == NATIVE) {
		return lambda->function(lval, quantity);
	}
	else if (lambda->type == CONSTRUCTED) {
		assert(lambda->argc == quantity);

		Env* newEnv = extendEnv(lambda->env);
		for(int i = 0; i < quantity; i++) {
			addVar(lambda->argv[i], lval[i], newEnv);
		}
	
		return eval(lambda->body, newEnv);
	}
	return NULL;
}

Lval* createLval() {
	Lval* lval = (Lval*)malloc(sizeof(Lval));
	lval->gcMark = false;
	addGcRef(getGlobalGcPool(), (void*)lval, LVAL);
	return lval;
}

void freeLval(Lval* lval) {
	if(lval->type == V_LAMBDA) {
		Lambda* lambda = lval->value.lambda;
		if(lambda->type == CONSTRUCTED) {
			for(int i = 0; i < lambda->argc; i++) {
				free(lambda->argv[i]);
			}
			freeAst(lambda->body);
		}
	}
	if(lval->type == V_SYMBOL) {
		free(lval->value.string);
	}
	// TODO: Check if lval is string
	free(lval);
}

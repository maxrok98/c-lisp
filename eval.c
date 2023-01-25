#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "eval.h"
#include "parser.h"
#include "gc.h"
#include "utils.h"

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
			case A_BOOLEAN:
				lval = createLval();
				lval->type = V_BOOLEAN;
				lval->value.boolean = atom->value.boolean;
				break;
			case A_SYMBOL:
				if(strcmp(atom->value.symbol, "null") == 0) {
					lval = createLval();
					lval->type = V_NULL;
				}
				else {
					lval = getVar(atom->value.symbol, env);
				}
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

					return value;
				}
				else if(strcmp(atom->value.symbol, "lambda") == 0) {
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
				else if(strcmp(atom->value.symbol, "if") == 0) {
					assert(list->quantity == 4);

					Lval* predicate = eval(list->astChildren[1], env);
					if(predicate->type == V_BOOLEAN && predicate->value.boolean == false) {
						return eval(list->astChildren[3], env);
					}
					return eval(list->astChildren[2], env);
				}
				else if(strcmp(atom->value.symbol, "set!") == 0) {
					assert(list->quantity == 3);
					assert(((Atom*)list->astChildren[1]->value)->type == A_SYMBOL);

					Lval* value = eval(list->astChildren[2], env);
					Atom* secondAtom = (Atom*)list->astChildren[1]->value;
					char* variable = secondAtom->value.symbol;

					setVar(variable, value, env);

					return value;
				}
				else if(strcmp(atom->value.symbol, "begin") == 0) {
					assert(list->quantity > 1);

					Lval* result;
					for(int i = 1; i < list->quantity; i++) {
						result = eval(list->astChildren[i], env);
					}
					return result;
				}
				else if(strcmp(atom->value.symbol, "load") == 0) {
					assert(list->quantity == 2);

					char* fileName = ((Atom*)list->astChildren[1]->value)->value.symbol;
					char* fileWithExtension = (char*)calloc(strlen(fileName) + 4, sizeof(char));
					strcpy(fileWithExtension, fileName);
					strcat(fileWithExtension, ".scm");

					loadFile(fileWithExtension, env);

					free(fileWithExtension);

					return NULL;
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

Lval* apply(Lval* operation, Lval** lvalList, int quantity, Env* env) {
	assert(operation != NULL);
	assert(operation->type == V_LAMBDA);
	Lambda* lambda = operation->value.lambda;
	if(lambda->type == NATIVE) {
		return lambda->function(lvalList, quantity);
	}
	else if (lambda->type == CONSTRUCTED) {
		assert(lambda->argc == quantity);

		Env* newEnv = extendEnv(lambda->env);
		for(int i = 0; i < quantity; i++) {
			addVar(lambda->argv[i], lvalList[i], newEnv);
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
	else if(lval->type == V_SYMBOL) {
		free(lval->value.string);
	}
	else if(lval->type == V_PAIR) {
		free(lval->value.pair);
	}
	// TODO: Check if lval is string
	free(lval);
}

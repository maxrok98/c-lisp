#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "eval.h"
#include "gc.h"

int hashFunction(char* key);
Var* createVariable(char* name, Lval* value);
void freeVar(Var* var);
void addNativeProc(char* symbol, Lval* lval, Lval* (*proc)(Lval**, int, Env*), Env* env);

Lval* addProc(Lval** lval, int quantity, Env* env);
Lval* minusProc(Lval** lval, int quantity, Env* env);
Lval* multiplyProc(Lval** lval, int quantity, Env* env);
Lval* divideProc(Lval** lval, int quantity, Env* env);
Lval* lessProc(Lval** lval, int quantity, Env* env);
Lval* moreProc(Lval** lval, int quantity, Env* env);
Lval* equalProc(Lval** lval, int quantity, Env* env);
Lval* andProc(Lval** lval, int quantity, Env* env);
Lval* orProc(Lval** lval, int quantity, Env* env);
Lval* notProc(Lval** lval, int quantity, Env* env);

Lval* getVar(char* name, Env* env) {
	int index = hashFunction(name);

	Var* varBucket = env->variables[index];
	while(varBucket != NULL) {
		if(strcmp(varBucket->name, name) == 0) {
			return varBucket->value;
		}
	
		varBucket = varBucket->next;
	}

	if(env->root == NULL) {
		return NULL;
	}
	else {
		return getVar(name, env->root);
	}
}

void addVar(char* name, Lval* value, Env* env) {
	int index = hashFunction(name);

	Var* var = createVariable(name, value);
	if(env->variables[index] == NULL) {
		env->variables[index] = var;
	}
	else {
		Var* varBucket = env->variables[index];
		if(strcmp(varBucket->name, name) == 0){
			var->next = varBucket->next;
			env->variables[index] = var;
			return;
		}
		while(varBucket->next != NULL) {
			if(strcmp(varBucket->next->name, name) == 0){
				var->next = varBucket->next->next;
				varBucket->next = var;
				return;
			}
			varBucket = varBucket->next;
		}
		varBucket->next = var;
	}
}

Env* setDefaultEnv() {
	Env* env = (Env*)malloc(sizeof(Env));
	env->root = NULL;
	memset(env->variables, 0, ENV_SIZE * sizeof(Var*));

	addGcRef(getGlobalGcPool(), (void*)env, ENV);

	addNativeProc("+", createLval(), &addProc, env);
	addNativeProc("-", createLval(), &minusProc, env);
	addNativeProc("*", createLval(), &multiplyProc, env);
	addNativeProc("/", createLval(), &divideProc, env);
	addNativeProc(">", createLval(), &moreProc, env);
	addNativeProc("<", createLval(), &lessProc, env);
	addNativeProc("=", createLval(), &equalProc, env);
	addNativeProc("and", createLval(), &andProc, env);
	addNativeProc("or", createLval(), &orProc, env);
	addNativeProc("not", createLval(), &notProc, env);

	// TODO: Add cons, car, cdr as native procs
	
	return env;
}

void addNativeProc(char* symbol, Lval* lval, Lval* (*proc)(Lval**, int, Env*), Env* env) {
	lval->type = V_LAMBDA;
	lval->value.lambda = (Lambda*)malloc(sizeof(Lambda));
	lval->value.lambda->type = NATIVE;
	lval->value.lambda->function = proc;
	addVar(symbol, lval, env);
}

Env* extendEnv(Env* env) {
	Env* newEnv = (Env*)malloc(sizeof(Env));
	newEnv->root = env;
	memset(newEnv->variables, 0, ENV_SIZE * sizeof(Var*));
	newEnv->gcMark = false;

	addGcRef(getGlobalGcPool(), (void*)newEnv, ENV);
	
	return newEnv;
}

Var* createVariable(char* name, Lval* value) {
	Var* var = (Var*)malloc(sizeof(Var));

	var->name = (char*)malloc(strlen(name) + 1);
	memcpy(var->name, name, sizeof(char) * strlen(name) + 1);

	var->value = value;

	var->next = NULL;

	return var;
}

int hashFunction(char* key) {
	char letter = key[0];
	return (letter & 0b1111) ^ 0b1010;
}

Lval* addProc(Lval** lval, int quantity, Env* env) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer += lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* minusProc(Lval** lval, int quantity, Env* env) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer -= lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* multiplyProc(Lval** lval, int quantity, Env* env) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer *= lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* divideProc(Lval** lval, int quantity, Env* env) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer /= lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* lessProc(Lval** lval, int quantity, Env* env) {
	assert(quantity == 2);
	assert(lval[0]->type == V_INTEGER);
	assert(lval[1]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->value.integer < lval[1]->value.integer;
	return lvalResult;
}

Lval* moreProc(Lval** lval, int quantity, Env* env) {
	assert(quantity == 2);
	assert(lval[0]->type == V_INTEGER);
	assert(lval[1]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->value.integer > lval[1]->value.integer;
	return lvalResult;
}

Lval* equalProc(Lval** lval, int quantity, Env* env) {
	assert(quantity == 2);
	assert(lval[0]->type == V_INTEGER);
	assert(lval[1]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->value.integer == lval[1]->value.integer;
	return lvalResult;
}

Lval* andProc(Lval** lval, int quantity, Env* env) {
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	for(int i = 0; i < quantity; i++) {
		if(lval[i]->value.boolean == false) {
			lvalResult->value.boolean = false;
			return lvalResult;
		}
	}
	lvalResult->value.boolean = true;
	return lvalResult;
}

Lval* orProc(Lval** lval, int quantity, Env* env) {
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	for(int i = 0; i < quantity; i++) {
		if(lval[i]->type != V_BOOLEAN || (lval[i]->type == V_BOOLEAN && lval[i]->value.boolean == true)) {
			lvalResult->value.boolean = true;
			return lvalResult;
		}
	}
	lvalResult->value.boolean = false;
	return lvalResult;
}

Lval* notProc(Lval** lval, int quantity, Env* env) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	if(lval[0]->type == V_BOOLEAN && lval[0]->value.boolean == false) {
		lvalResult->value.boolean = true;
		return lvalResult;
	}
	lvalResult->value.boolean = false;
	return lvalResult;
}

void freeEnv(Env* env) {
	for(int i = 0; i < ENV_SIZE; i++) {
		Var* varBucket = env->variables[i];
		if(varBucket != NULL) {
			freeVar(varBucket);
		}
	}
	free(env);
}

void freeVar(Var* var) {
	if(var->next != NULL) {
		freeVar(var->next);
	}
	free(var->name);
	free(var);
}

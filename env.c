#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "eval.h"
#include "gc.h"

int hashFunction(char* key);
Var* createVariable(char* name, Lval* value);
void freeVar(Var* var);

Lval* addProc(Lval** lval, int quantity);
Lval* minusProc(Lval** lval, int quantity);
Lval* multiplyProc(Lval** lval, int quantity);
Lval* divideProc(Lval** lval, int quantity);

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

	Lval* lvalAdd = createLval();
	lvalAdd->type = V_LAMBDA;
	lvalAdd->value.lambda = (Lambda*)malloc(sizeof(Lambda));
	lvalAdd->value.lambda->type = NATIVE;
	lvalAdd->value.lambda->function = &addProc;
	addVar("+", lvalAdd, env);

	Lval* lvalMinus = createLval();
	lvalMinus->type = V_LAMBDA;
	lvalMinus->value.lambda = (Lambda*)malloc(sizeof(Lambda));
	lvalMinus->value.lambda->type = NATIVE;
	lvalMinus->value.lambda->function = &minusProc;
	addVar("-", lvalMinus, env);

	Lval* lvalMultiply = createLval();
	lvalMultiply->type = V_LAMBDA;
	lvalMultiply->value.lambda = (Lambda*)malloc(sizeof(Lambda));
	lvalMultiply->value.lambda->type = NATIVE;
	lvalMultiply->value.lambda->function = &multiplyProc;
	addVar("*", lvalMultiply, env);

	Lval* lvalDivide = createLval();
	lvalDivide->type = V_LAMBDA;
	lvalDivide->value.lambda = (Lambda*)malloc(sizeof(Lambda));
	lvalDivide->value.lambda->type = NATIVE;
	lvalDivide->value.lambda->function = &divideProc;
	addVar("/", lvalDivide, env);

	// TODO: Add cons, car, cdr as native procs
	
	return env;
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

Lval* addProc(Lval** lval, int quantity) {
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

Lval* minusProc(Lval** lval, int quantity) {
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

Lval* multiplyProc(Lval** lval, int quantity) {
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

Lval* divideProc(Lval** lval, int quantity) {
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

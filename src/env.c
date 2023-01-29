#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "eval.h"
#include "gc.h"
#include "std_procs.h"

int hashFunction(char* key);
Var* createVariable(char* name, Lval* value);
void freeVar(Var* var);
void addNativeProc(char* symbol, int argc, Lval* (*proc)(Lval**, int), Env* env);

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
		ERROR("%s variable not bound", name);
	}

	return getVar(name, env->root);
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

Lval* setVar(char* name, Lval* value, Env* env) {
	int index = hashFunction(name);

	Var* varBucket = env->variables[index];
	while(varBucket != NULL) {
		if(strcmp(varBucket->name, name) == 0) {
			varBucket->value = value;
			return value;
		}
	
		varBucket = varBucket->next;
	}

	if(env->root == NULL) {
		ERROR("%s variable not bound", name);
	}

	return setVar(name, value, env->root);
}

Env* setDefaultEnv() {
	Env* env = (Env*)malloc(sizeof(Env));
	env->root = NULL;
	memset(env->variables, 0, ENV_SIZE * sizeof(Var*));

	addGcRef(getGlobalGcPool(), (void*)env, ENV);

	addNativeProc("+", -1, &addProc, env);
	addNativeProc("-", -1, &minusProc, env);
	addNativeProc("*", -1, &multiplyProc, env);
	addNativeProc("/", -1, &divideProc, env);
	addNativeProc(">", -1, &moreProc, env);
	addNativeProc("<", -1, &lessProc, env);
	addNativeProc("=", 2, &equalProc, env);
	addNativeProc("and", -1, &andProc, env);
	addNativeProc("or", -1, &orProc, env);
	addNativeProc("not", -1, &notProc, env);
	addNativeProc("cons", 2, &consProc, env);
	addNativeProc("car", 1, &carProc, env);
	addNativeProc("cdr", 1, &cdrProc, env);
	addNativeProc("list", -1, &listProc, env);
	addNativeProc("set-car!", 2, &setCarProc, env);
	addNativeProc("set-cdr!", 2, &setCdrProc, env);
	addNativeProc("number?", 1, &numberPredProc, env);
	addNativeProc("boolean?", 1, &booleanPredProc, env);
	addNativeProc("pair?", 1, &pairPredProc, env);
	addNativeProc("null?", 1, &nullPredProc, env);
	addNativeProc("lambda?", 1, &lambdaPredProc, env);
	
	return env;
}

void addNativeProc(char* symbol, int argc, Lval* (*proc)(Lval**, int), Env* env) {
	Lval* lval = createLval();
	lval->type = V_LAMBDA;
	lval->value.lambda = (Lambda*)malloc(sizeof(Lambda));
	lval->value.lambda->type = NATIVE;
	lval->value.lambda->function = proc;
	lval->value.lambda->argc = argc;
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

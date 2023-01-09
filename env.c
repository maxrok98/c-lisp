#include <stdlib.h>
#include <string.h>

#include "eval.h"

int hashFunction(char* key);
Var* createVariable(char* name, Lval* value);
void changeVar(Var* var, Lval* lval);

Lval getVar(char* name, Env* env) {
	int index = hashFunction(name);
	
	Var* varBucket = env->variables[index];
	if(varBucket != NULL) {
		do {
			if(strcmp(varBucket->name, name) == 0) {
				return *(varBucket->value);
			}
		
			varBucket = varBucket->next;
		}
		while(varBucket != NULL);
	}

	Lval lval;
	lval.type = V_NULL;
	return lval;
}

void addVar(char* name, Lval* value, Env* env) {
	int index = hashFunction(name);

	if(env->variables[index] == NULL) {
		env->variables[index] = createVariable(name, value);
	}
	else {
		Var* varBucket = env->variables[index];
		do {
			if(strcmp(varBucket->name, name) == 0){
				changeVar(varBucket, value);
				return;
			}
			varBucket = varBucket->next;
		}
		while(varBucket->next != NULL);
		varBucket->next = createVariable(name, value);
	}
}

Env* setDefaultEnv() {
	Env* env = (Env*)malloc(sizeof(Env));
	env->root = NULL;
	
	return env;
}

Var* createVariable(char* name, Lval* value) {
	Var* var = (Var*)malloc(sizeof(Var));

	var->name = (char*)malloc(strlen(name) + 1);
	memcpy(var->name, name, sizeof(char) * strlen(name) + 1);

	var->value = (Lval*)malloc(sizeof(Lval));
	memcpy(var->value, value, sizeof(Lval));

	var->next = NULL;

	return var;
}

void changeVar(Var* var, Lval* lval) {
	// TODO: Leak of memory if value of variable accessible only via one variable
	memcpy(var->value, lval, sizeof(Lval));
}

int hashFunction(char* key) {
	char letter = key[0];
	return (letter & 0b1111) ^ 0b1010;
}

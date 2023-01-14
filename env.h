#pragma once

//#include "eval.h"

#define ENV_SIZE 16

typedef struct Var {
	char* name;
	Lval* value;
	struct Var* next;
} Var;

typedef struct Env {
	struct Env* root;
	Var* variables[ENV_SIZE];
} Env;


Lval* getVar(char* name, Env* env);
void addVar(char* name, Lval* value, Env* env);
void setVar(char* name, Lval* value, Env* env);
Env* setDefaultEnv();
Env* extendEnv(Env* env); // register referense in GC pool
void freeEnv(Env* env);


#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "test.h"
#include "../src/tokenizer.h"
#include "../src/parser.h"
#include "../src/eval.h"
#include "../src/env.h"
#include "../src/utils.h"
#include "../src/gc.h"

static int env_shoud_add_var() {
	Env* env = (Env*)calloc(1, sizeof(Env));

	Lval* lval = (Lval*)malloc(sizeof(Lval));
	lval->type = V_INTEGER;
	lval->value.integer = 42;

	addVar("a", lval, env);
	Lval* lvalResult = getVar("a", env);

	ASSERT_TRUE(lval == lvalResult);

	freeLval(lval);
	freeEnv(env);
	return 0;
}

static int env_shoud_return_var_from_root() {
	Env* env = (Env*)calloc(1, sizeof(Env));

	Env* envExt = (Env*)calloc(1, sizeof(Env));
	envExt->root = env;

	Lval* lvala = (Lval*)malloc(sizeof(Lval));
	lvala->type = V_INTEGER;
	lvala->value.integer = 42;
	addVar("a", lvala, env);

	Lval* lvalb = (Lval*)malloc(sizeof(Lval));
	lvalb->type = V_INTEGER;
	lvalb->value.integer = 69;
	addVar("b", lvalb, envExt);
	
	Lval* lvalResult = getVar("a", envExt);

	ASSERT_TRUE(lvala == lvalResult);

	freeLval(lvala);
	freeLval(lvalb);
	freeEnv(env);
	freeEnv(envExt);
	return 0;
}

static int env_shoud_return_var_from_extended() {
	Env* env = (Env*)calloc(1, sizeof(Env));

	Env* envExt = (Env*)calloc(1, sizeof(Env));
	envExt->root = env;

	Lval* lval = (Lval*)malloc(sizeof(Lval));
	lval->type = V_INTEGER;
	lval->value.integer = 42;
	addVar("b", lval, env);

	Lval* lvalb = (Lval*)malloc(sizeof(Lval));
	lvalb->type = V_INTEGER;
	lvalb->value.integer = 69;
	addVar("b", lvalb, envExt);
	
	Lval* lvalResult = getVar("b", envExt);

	ASSERT_TRUE(lvalb == lvalResult);

	freeLval(lval);
	freeLval(lvalb);
	freeEnv(env);
	freeEnv(envExt);
	return 0;
}

static int env_shoud_set_var_in_extended() {
	Env* env = (Env*)calloc(1, sizeof(Env));

	Env* envExt = (Env*)calloc(1, sizeof(Env));
	envExt->root = env;

	Lval* lval = (Lval*)malloc(sizeof(Lval));
	lval->type = V_INTEGER;
	lval->value.integer = 42;
	addVar("b", lval, env);

	Lval* lvalb = (Lval*)malloc(sizeof(Lval));
	lvalb->type = V_INTEGER;
	lvalb->value.integer = 69;
	addVar("b", lvalb, envExt);

	Lval* lvalb2 = (Lval*)malloc(sizeof(Lval));
	lvalb2->type = V_INTEGER;
	lvalb2->value.integer = 69;
	setVar("b", lvalb2, envExt);
	
	Lval* lvalResult = getVar("b", envExt);

	ASSERT_TRUE(lvalb2 == lvalResult);

	freeLval(lval);
	freeLval(lvalb);
	freeLval(lvalb2);
	freeEnv(env);
	freeEnv(envExt);
	return 0;
}

static int env_shoud_set_var_in_root() {
	Env* env = (Env*)calloc(1, sizeof(Env));

	Env* envExt = (Env*)calloc(1, sizeof(Env));
	envExt->root = env;

	Lval* lvala = (Lval*)malloc(sizeof(Lval));
	lvala->type = V_INTEGER;
	lvala->value.integer = 42;
	addVar("a", lvala, env);

	Lval* lvalb = (Lval*)malloc(sizeof(Lval));
	lvalb->type = V_INTEGER;
	lvalb->value.integer = 69;
	addVar("b", lvalb, envExt);

	Lval* lvala2 = (Lval*)malloc(sizeof(Lval));
	lvala2->type = V_INTEGER;
	lvala2->value.integer = 42;
	setVar("a", lvala2, envExt);
	
	Lval* lvalResult = getVar("a", envExt);

	ASSERT_TRUE(lvala2 == lvalResult);

	freeLval(lvala);
	freeLval(lvala2);
	freeLval(lvalb);
	freeEnv(env);
	freeEnv(envExt);
	return 0;
}
static void env_tests() {
	TEST(env_shoud_add_var);
	TEST(env_shoud_return_var_from_root);
	TEST(env_shoud_return_var_from_extended);
	TEST(env_shoud_set_var_in_extended);
	TEST(env_shoud_set_var_in_root);
}

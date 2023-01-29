#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "test.h"
#include "../src/tokenizer.h"
#include "../src/parser.h"
#include "../src/eval.h"
#include "../src/env.h"
#include "../src/gc.h"

#define lambda(return_type, function_body) \
({ \
	return_type __fn__ function_body \
		__fn__; \
})

#define EXEC_INSIDE_GC(test)\
	setGlobalGcPool(createGcPool());\
	Env* env = (Env*)calloc(1, sizeof(Env));\
	int result = test(env);\
	freeEnv(env);\
	freeGcPool(getGlobalGcPool());\
	return result;

static int gc_should_free_val() {
	EXEC_INSIDE_GC(
		lambda (int, (Env* env) {
			Lval* lval = createLval();
			lval->type = V_INTEGER;
			lval->value.integer = 42;
			
			garbageCollect(env);
			// there is no way to check if value is freed because pointer still points to same place
			// so just checking if value are not the same
			ASSERT_TRUE(lval->type != V_INTEGER);
			ASSERT_TRUE(lval->value.integer != 42);

			return 0;
		})
	);
}

static int gc_should_not_free_val_from_env() {
	EXEC_INSIDE_GC(
		lambda (int, (Env* env) {
			Lval* lval = createLval();
			lval->type = V_INTEGER;
			lval->value.integer = 42;
			addVar("a", lval, env);
			
			garbageCollect(env);

			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 42);
			lval = getVar("a", env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 42);

			return 0;
		})
	);
}

static int gc_should_free_env_if_env_is_unreachable() {
	EXEC_INSIDE_GC(
		lambda (int, (Env* env) {
			Env* envExtended = extendEnv(env);
			Lval* lval = createLval();
			lval->type = V_INTEGER;
			lval->value.integer = 42;
			addVar("a", lval, envExtended);
			
			garbageCollect(env);

			ASSERT_TRUE(lval->type != V_INTEGER);
			ASSERT_TRUE(lval->value.integer != 42);
			ASSERT_TRUE(envExtended->root != env);

			return 0;
		})
	);
}

static int gc_should_not_free_env_if_env_is_reachable() {
	EXEC_INSIDE_GC(
		lambda (int, (Env* env) {
			Env* envExtended = extendEnv(env);

			Lval* lval = createLval();
			lval->type = V_LAMBDA;
			Lambda* lambda = (Lambda*)malloc(sizeof(Lambda));
			lambda->type = CONSTRUCTED;
			lambda->env = envExtended;
			lval->value.lambda = lambda;

			addVar("lamb", lval, env);
			
			garbageCollect(env);

			ASSERT_TRUE(envExtended->root == env);

			freeEnv(envExtended);
			return 0;
		})
	);
}

static int gc_should_not_free_values_from_list() {
	EXEC_INSIDE_GC(
		lambda (int, (Env* env) {
			Lval* lpair1 = createLval();
			lpair1->type = V_PAIR;
			Pair* pair1 = (Pair*)malloc(sizeof(Pair));
			lpair1->value.pair = pair1;

			Lval* lpair2 = createLval();
			lpair2->type = V_PAIR;
			Pair* pair2 = (Pair*)malloc(sizeof(Pair));
			lpair2->value.pair = pair2;

			Lval* a = createLval();
			a->type = V_INTEGER;
			a->value.integer = 42;

			Lval* b = createLval();
			b->type = V_INTEGER;
			b->value.integer = 69;

			Lval* n = createLval();
			n->type = V_NULL;

			pair1->car = a;
			pair1->cdr = lpair2;

			pair2->car = b;
			pair2->cdr = n;

			addVar("li", lpair1, env);
			
			garbageCollect(env);

			ASSERT_TRUE(b->type == V_INTEGER);
			ASSERT_TRUE(b->value.integer == 69);

			Lval* nu = createLval();
			nu->type = V_NULL;

			// shoud free list if its unreachable
			addVar("li", nu, env);

			garbageCollect(env);

			ASSERT_TRUE(b->type != V_INTEGER);
			ASSERT_TRUE(b->value.integer != 69);
			
			return 0;
		})
	);
}

static void gc_tests() {
	TEST(gc_should_free_val);
	TEST(gc_should_not_free_val_from_env);
	TEST(gc_should_free_env_if_env_is_unreachable);
	TEST(gc_should_not_free_env_if_env_is_reachable);
	TEST(gc_should_not_free_values_from_list);
}

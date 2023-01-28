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

#define lambda(return_type, function_body) \
({ \
	return_type __fn__ function_body \
		__fn__; \
})

#define EXEC_INSIDE_DEFAULT_ENV(expression, test)\
	setGlobalGcPool(createGcPool());\
	Env* env = setDefaultEnv();\
	Tokenizer* tokenizer = generateTokenizer(expression);\
	Ast* ast = parse(tokenizer);\
	int result = test(ast, env);\
	free(tokenizer);\
	freeAst(ast);\
	garbageCollect(env);\
	freeGcPool(getGlobalGcPool());\
	freeEnv(env);\
	return result;


static int eval_should_return_addition_result() {
	EXEC_INSIDE_DEFAULT_ENV("(+ 53 16)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 69);

			return 0;
		})
	);
}


static int eval_should_return_nested_arithmetic_result() {
	EXEC_INSIDE_DEFAULT_ENV("(+ 53 16 (- 42 42) (* 2 2) (/ 8 2))", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 77);

			return 0;
		})
	);
}

static int eval_should_return_boolean_from_arithmetic_predicate() {
	EXEC_INSIDE_DEFAULT_ENV("(< 2 4)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_BOOLEAN);
			ASSERT_TRUE(lval->value.boolean == true);
			
			return 0;
		})
	);
}

static int eval_should_return_boolean_from_arithmetic_predicate_with_logic_operation() {
	EXEC_INSIDE_DEFAULT_ENV("(and (< 2 4) #f)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_BOOLEAN);
			ASSERT_TRUE(lval->value.boolean == false);
			
			return 0;
		})
	);
}

static int eval_should_return_lambda() {
	EXEC_INSIDE_DEFAULT_ENV("(lambda (a b) (+ a b))", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_LAMBDA);

			Lambda* lambda = lval->value.lambda;
			ASSERT_TRUE(lambda->type == CONSTRUCTED);
			ASSERT_TRUE(lambda->argc == 2);
			ASSERT_TRUE(strcmp(lambda->argv[0], "a") == 0)
			ASSERT_TRUE(strcmp(lambda->argv[1], "b") == 0)
			
			return 0;
		})
	);
}

static int eval_should_return_sum_of_square_from_lambda() {
	EXEC_INSIDE_DEFAULT_ENV("((lambda (a b) (+ (* a a) (* b b))) 3 4 )", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 25);
			
			return 0;
		})
	);
}

static int eval_should_eval_first_expresion_if_predicate_is_true() {
	EXEC_INSIDE_DEFAULT_ENV("(if (< 2 4) (+ 2 2) (- 2 2))", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 4);
			
			return 0;
		})
	);
}

static int eval_should_eval_second_expresion_if_predicate_is_false() {
	EXEC_INSIDE_DEFAULT_ENV("(if (> 2 4) (+ 2 2) (- 2 2))", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 0);
			
			return 0;
		})
	);
}

static int eval_should_pair() {
	EXEC_INSIDE_DEFAULT_ENV("(cons 1 2)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_PAIR);
			Pair* pair = lval->value.pair;
			ASSERT_TRUE(pair->car->value.integer == 1);
			ASSERT_TRUE(pair->cdr->value.integer == 2);
			
			return 0;
		})
	);
}

static int eval_should_return_car_of_pair() {
	EXEC_INSIDE_DEFAULT_ENV("(car (cons 1 2))", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 1);
			
			return 0;
		})
	);
}

static int eval_should_return_cdr_of_pair() {
	EXEC_INSIDE_DEFAULT_ENV("(cdr (cons 1 2))", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 2);
			
			return 0;
		})
	);
}

static int eval_should_define_var_in_env() {
	EXEC_INSIDE_DEFAULT_ENV("(begin (define a 42) a)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 42);
			
			return 0;
		})
	);
}

static int eval_should_redefine_var_in_env() {
	EXEC_INSIDE_DEFAULT_ENV("(begin (define a 42) (define a 69) a)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 69);
			
			return 0;
		})
	);
}

static int eval_should_set_val_to_var_in_env() {
	EXEC_INSIDE_DEFAULT_ENV(
		"(begin (define a 42) (set! a 69) a)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 69);
			
			return 0;
		})
	);
}

static int eval_should_set_val_to_var_in_lexical_scope() {
	EXEC_INSIDE_DEFAULT_ENV(
		"(begin (define a 42) ((lambda (a) (set! a 69)) 5) a)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_INTEGER);
			ASSERT_TRUE(lval->value.integer == 42);
			
			return 0;
		})
	);
}

static int eval_should_set_car_val_to_var() {
	EXEC_INSIDE_DEFAULT_ENV(
		"(begin (define a (cons 1 2)) (set-car! a 3) a)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_PAIR);
			Pair* pair = lval->value.pair;
			ASSERT_TRUE(pair->car->value.integer == 3);
			
			return 0;
		})
	);
}

static int eval_should_set_cdr_val_to_var() {
	EXEC_INSIDE_DEFAULT_ENV(
		"(begin (define a (cons 1 2)) (set-cdr! a 3) a)", 
		lambda (int, (Ast* ast, Env* env) {
			Lval* lval = eval(ast, env);
			ASSERT_TRUE(lval->type == V_PAIR);
			Pair* pair = lval->value.pair;
			ASSERT_TRUE(pair->cdr->value.integer == 3);
			
			return 0;
		})
	);
}

static void eval_tests() {
	TEST(eval_should_return_addition_result);
	TEST(eval_should_return_nested_arithmetic_result);
	TEST(eval_should_return_boolean_from_arithmetic_predicate);
	TEST(eval_should_return_boolean_from_arithmetic_predicate_with_logic_operation);
	TEST(eval_should_return_lambda);
	TEST(eval_should_return_sum_of_square_from_lambda);
	TEST(eval_should_eval_first_expresion_if_predicate_is_true);
	TEST(eval_should_eval_second_expresion_if_predicate_is_false);
	TEST(eval_should_pair);
	TEST(eval_should_return_car_of_pair);
	TEST(eval_should_return_cdr_of_pair);
	TEST(eval_should_define_var_in_env);
	TEST(eval_should_redefine_var_in_env);
	TEST(eval_should_set_val_to_var_in_env);
	TEST(eval_should_set_val_to_var_in_lexical_scope);
	TEST(eval_should_set_car_val_to_var);
	TEST(eval_should_set_cdr_val_to_var);
}

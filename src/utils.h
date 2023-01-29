#pragma once

#include "eval.h"
#include "env.h"

#define EXPRESSION_SIZE 1024
#define LINE_SIZE (EXPRESSION_SIZE / 2)

#define S_INTEGER "integer"
#define S_REAL		"real"
#define S_BOOLEAN "boolean"
#define S_STRING	"string"
#define S_LAMBDA	"lambda"
#define S_SYMBOL	"symbol"
#define S_PAIR		"pair"
#define S_NULL		"null"
#define S_ERR			"eror"

int checkParensParity(char* input);
void printLval(Lval* lval);
void readInputExpression(char* expressionInput);
void loadFile(char* fileName, Env* env);
bool stringIsEmpty(char* input);
char* typeToString(Lval* lval);

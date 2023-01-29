#pragma once

#include "eval.h"
#include "env.h"

#define EXPRESSION_SIZE 1024
#define LINE_SIZE (EXPRESSION_SIZE / 2)

int checkParensParity(char* input);
void printLval(Lval* lval);
void readInputExpression(char* expressionInput);
void loadFile(char* fileName, Env* env);
bool stringIsEmpty(char* input);

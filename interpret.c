#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"
#include "eval.h"
//#include "env.h"
#include "utils.h"
#include "gc.h"
#include "debug_utils.h"

#define EXPRESSION_SIZE 128
#define LINE_SIZE (EXPRESSION_SIZE / 2)

int main(int argc, char** argv) {
	printf("Scheme interpreter:\n");

	setGlobalGcPool(createGcPool());
	Env* env = setDefaultEnv();

	char expressionInput[EXPRESSION_SIZE];
	int savedExpressionLength = 0;
	char lineInput[LINE_SIZE];
	while(true){
		if(savedExpressionLength == 0) fputs("> ", stdout);
		fgets(lineInput, LINE_SIZE, stdin);
		int actualLineSize = strlen(lineInput);
		if(actualLineSize + savedExpressionLength < EXPRESSION_SIZE) {
			strcpy(expressionInput + savedExpressionLength, lineInput);
			savedExpressionLength += actualLineSize;			
		}
		if(checkParensParity(expressionInput) > 0) {
			continue;
		}
		else if(checkParensParity(expressionInput) < 0) {
			printf("Error: unexpected close paren ')'");
			savedExpressionLength = 0;
			continue;
		}

		Tokenizer* tokenizer = generateTokenizer(expressionInput);
		Ast* ast = parse(tokenizer);

		Lval* lval = eval(ast, env);
		if(lval->type == V_INTEGER){
			printf("%d\n", lval->value.integer);
		}

		garbageCollect(env);
		free(tokenizer);
		freeAst(ast);
		savedExpressionLength = 0;
	}
}


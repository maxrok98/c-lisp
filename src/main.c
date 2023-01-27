#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"
#include "eval.h"
#include "env.h"
#include "utils.h"
#include "gc.h"
#include "debug_utils.h"

int main(int argc, char** argv) {
	printf("Scheme interpreter:\n");

	setGlobalGcPool(createGcPool());
	Env* env = setDefaultEnv();

	if(argc > 1 && strcmp(argv[1], "-f") == 0) {
		for(int i = 2; i < argc; i++) {
			loadFile(argv[i], env);
		}
	}

	char expressionInput[EXPRESSION_SIZE];
	while(true){
		readInputExpression(expressionInput);

		Tokenizer* tokenizer = generateTokenizer(expressionInput);
		Ast* ast = parse(tokenizer);

		Lval* lval = eval(ast, env);
		printLval(lval);

		free(tokenizer);
		freeAst(ast);
		garbageCollect(env);
	}
}


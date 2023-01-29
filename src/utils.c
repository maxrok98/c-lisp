#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "gc.h"

bool isList(Lval* lval);

/*
 * >0 if parens are not closed
 * 0  if all parens closed
 * -1 if unexpected close paren
 */
int checkParensParity(char* input) {
	int parity = 0;
	while(*input) {
		if(*input == '(')
			parity++;
		else if(*input == ')') {
			parity--;
			if(parity < 0) 
				return parity;
		}
		input++;
	}
	return parity;
}

void printValue(Lval* lval) {
	if(lval == NULL) return;
	if(lval->type == V_SYMBOL) {
		printf("%s", lval->value.string);
	}
	else if(lval->type == V_INTEGER){
		printf("%d", lval->value.integer);
	}
	else if(lval->type == V_LAMBDA) {
		Lambda* lambda = lval->value.lambda;
		if(lambda->type == NATIVE) {
			printf("#[native procedure]");
		}
		else if(lambda->type == CONSTRUCTED) {
			printf("#[\u03bb (");
			for(int i = 0; i < lambda->argc; i++) {
				printf("%s ", lambda->argv[i]);
				if(i == lambda->argc - 1) printf("\b");
			}
			printf(")]");
		}
	}
	else if(lval->type == V_PAIR) {
		if(isList(lval)) {
			printf("(");

			while(lval->type != V_NULL) {
				printLval(lval->value.pair->car);
				printf(" ");
				lval = lval->value.pair->cdr;
			}
			printf("\b)");
		}
		else {
			printf("(");
			printLval(lval->value.pair->car);
			printf(" . ");
			printLval(lval->value.pair->cdr);
			printf(")");
		}
	}
	else if(lval->type == V_BOOLEAN) {
		if(lval->value.boolean){
			printf("#t");
		}
		else {
			printf("#f");
		}
	}
	else if(lval->type == V_NULL) {
		printf("null");
	}
}

void printLval(Lval* lval) {
	printValue(lval);
	printf("\n");
}

bool isList(Lval* lval) {
	Pair* pair = lval->value.pair;
	while(pair->cdr->type == V_PAIR || pair->cdr->type == V_NULL) {
		if(pair->cdr->type == V_NULL) {
			return true;
		}
		pair = pair->cdr->value.pair;
	}
	return false;
}

void loadFile(char* fileName, Env* env) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		return;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* fileExpression = malloc(fsize + 1);
	fread(fileExpression, fsize, 1, f);
	fclose(f);

	fileExpression[fsize] = 0;

	Tokenizer* tokenizer = generateTokenizer(fileExpression);
	Ast* ast;
	while((ast = parse(tokenizer))) {
		eval(ast, env);
		freeAst(ast);
	}
	garbageCollect(env);
}

void readInputExpression(char* expressionInput) {
	int savedExpressionLength = 0;
	char lineInput[LINE_SIZE];

	do {
		if(savedExpressionLength == 0) fputs("> ", stdout);

		fgets(lineInput, LINE_SIZE, stdin);

		int actualLineSize = strlen(lineInput);
		if(actualLineSize + savedExpressionLength < EXPRESSION_SIZE) {
			strcpy(expressionInput + savedExpressionLength, lineInput);
			savedExpressionLength += actualLineSize;			
		}

		if(checkParensParity(expressionInput) < 0) {
			printf("Error: unexpected close paren ')'");
			savedExpressionLength = 0;
		}
	} 
	while(checkParensParity(expressionInput) > 0 || checkParensParity(expressionInput) < 0);
}

bool stringIsEmpty(char* input) {
	while(*input != '\0') {
		if(!isspace(*input)) {
			return false;
		}
		input++;
	}
	return true;
}

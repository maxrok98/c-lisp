#include <stdbool.h>
#include <stdio.h>

#include "utils.h"

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

void printLval(Lval* lval) {
	if(lval->type == V_SYMBOL) {
		printf("%s", lval->value.string);
	}
	else if(lval->type == V_INTEGER){
		printf("%d", lval->value.integer);
	}
	else if(lval->type == V_LAMBDA) {
		Lambda* lambda = lval->value.lambda;
		if(lambda->type == NATIVE) {
			printf("<native procedure>");
		}
		else if(lambda->type == CONSTRUCTED) {
			printf("<\u03bb (");
			for(int i = 0; i < lambda->argc; i++) {
				printf("%s ", lambda->argv[i]);
			}
			printf("\b)>");
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
	else if(lval->type == V_NULL) {
		printf("null");
	}
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

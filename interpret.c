#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "tokenizer.h"

#define EXPRESSION_SIZE 128

void printToken(Token* token);

int main(int argc, char** argv) {
	printf("Scheme interpreter:\n");

	char input[EXPRESSION_SIZE];
	while(true){
		fputs("> ", stdout);
		fgets(input, EXPRESSION_SIZE, stdin);

		Tokenizer* tokenizer = generateTokenizer(input);
		Token* token;
		while(token = nextToken(tokenizer)) {
			printToken(token);
			free(token->value);
			free(token);
		}

		free(tokenizer);
		break;
	}
}

void printToken(Token* token) {
	switch(token->tokenType){
		case L_PAREN:
			printf("Token: L_PAREN - %s\n", "(");
			break;
		case R_PAREN:
			printf("Token: R_PAREN - %s\n", ")");
			break;
		case INTEGER:
			printf("Token: INTEGER - %s\n", token->value);
			break;
		case REAL:
			printf("Token: REAL - %s\n", token->value);
			break;
		case SYMBOL:
			printf("Token: SYMBOL - %s\n", token->value);
			break;
	}
}


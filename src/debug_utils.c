#include <stdio.h>
#include "tokenizer.h"
#include "parser.h"

void printDepth(char* c, int depth) {
	for(int i = 0; i < depth; i++){
		printf(c);
	}
}

void printAstAsTree(Ast* ast, int depth) {
	if(ast->type == ATOM) {

		printDepth(" | ", depth);

		Atom* atom = (Atom*)ast->value;
		switch(atom->type) {
			case A_INTEGER:
				printf("%ld\n", atom->value.integer);
				break;
			case A_REAL:
				printf("%lf\n", atom->value.real);
				break;
			case A_SYMBOL:
				printf("%s\n", atom->value.symbol);
				break;
		}
	}
	else if(ast->type == LIST) {
		printDepth("---", depth + 1);
		printf("\n");

		List* list = (List*)ast->value;
		for(int i = 0; i < list->quantity; i++) {
			printAstAsTree(list->astChildren[i], depth + 1);
		}

		printDepth("---", depth + 1);
		printf("\n");
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

void printTokenizer(Tokenizer* tokenizer) {
	Token* token;
	while(token = nextToken(tokenizer)) {
		printToken(token);
		freeToken(token);
	}
}


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "tokenizer.h"

bool isdelimiter(char symbol);
bool isinteger(char* start, char* end);
bool isreal(char* start, char* end);

/*
 * Tokenizer should be freed when not needed
 */
Tokenizer* generateTokenizer(char* expression) {
	Tokenizer* tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));
	tokenizer->expression = expression;
	tokenizer->expressionLength = strlen(expression);
	tokenizer->start = 0;
	tokenizer->end = 0;

	return tokenizer;
}

/*
 * Token should be freed when not needed
 */
Token* nextToken(Tokenizer* tokenizer) {
	assert(tokenizer->start == tokenizer->end);
	while(isspace(tokenizer->expression[tokenizer->start])) {
		tokenizer->start++; tokenizer->end++;
		if(tokenizer->start >= tokenizer->expressionLength) return NULL;
	}

	Token* token = (Token*)malloc(sizeof(Token));
	if(tokenizer->expression[tokenizer->start] == '('){
		token->tokenType = L_PAREN;
		tokenizer->start++;
		tokenizer->end = tokenizer->start;
		return token;
	}
	if(tokenizer->expression[tokenizer->start] == ')'){
		token->tokenType = R_PAREN;
		tokenizer->start++;
		tokenizer->end = tokenizer->start;
		return token;
	}

	while(!isspace(tokenizer->expression[tokenizer->end]) &&
				!isdelimiter(tokenizer->expression[tokenizer->end])) {
		tokenizer->end++;
	}
	tokenizer->end--;

	// TODO: add reals
	if(isinteger(tokenizer->expression + tokenizer->start,
							 tokenizer->expression + tokenizer->end)) {
		token->tokenType = INTEGER;
	}
	else {
		token->tokenType = SYMBOL;
	}

	int tokenLength = tokenizer->end - tokenizer->start + 1;
	token->value = (char*)calloc(tokenLength + 1, sizeof(char));
	memcpy(token->value, tokenizer->expression + tokenizer->start, tokenLength);
	tokenizer->start = ++tokenizer->end;

	return token;
}

void freeToken(Token* token) {
	if(token->value != NULL)
		free(token->value);
	free(token);
}

bool isdelimiter(char symbol) {
	return symbol == '(' || symbol == ')';
}

bool isinteger(char* start, char* end) {
	bool wasNumber = false;
	while(end > start) {
		if(!isdigit(*end)) {
			return false;
		}

		wasNumber = true;
		end--;
	}

	if(isdigit(*end) || (*end == '-' && wasNumber)) {
		return true;
	}
	return false;	
}


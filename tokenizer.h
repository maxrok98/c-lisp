#pragma once

typedef enum TokenType {
	L_PAREN,
	R_PAREN,
	INTEGER,
	REAL,
	SYMBOL,
	ERR
} TokenType;

typedef struct Token {
	TokenType tokenType;
	char* value;
} Token;

typedef struct Tokenizer {
	char* expression;
	int 	expressionLength;

	/* window indexes */
	int start;
	int end;
} Tokenizer;

Tokenizer* generateTokenizer(char* expression);
Token* nextToken(Tokenizer* tokenizer);
void freeToken(Token* token);


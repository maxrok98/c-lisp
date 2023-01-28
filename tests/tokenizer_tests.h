#include <string.h>

#include "test.h"
#include "../src/tokenizer.h"

static int tokenizer_check_simple_expression() {
	char* expression = "(1 2 3)";
	Tokenizer* tokenizer = generateTokenizer(expression);

	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer) == NULL);

	return 0;
}

static int tokenizer_check_multiline_with_spaces_expression() {
	char* expression = "  (   1 2 \n \t 3)";
	Tokenizer* tokenizer = generateTokenizer(expression);

	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer) == NULL);

	return 0;
}

static int tokenizer_check_multiline_expression_in_line() {
	char* expression = "(3) (5) 6";
	Tokenizer* tokenizer = generateTokenizer(expression);

	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer) == NULL);

	return 0;
}

static int tokenizer_check_nested_expressions() {
	char* expression = "(3 (5) 6)";
	Tokenizer* tokenizer = generateTokenizer(expression);

	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == INTEGER);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer) == NULL);

	return 0;
}

static int tokenizer_check_boolean_expressions() {
	char* expression = "(#t #f)";
	Tokenizer* tokenizer = generateTokenizer(expression);

	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == BOOLEAN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == BOOLEAN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer) == NULL);

	return 0;
}

static int tokenizer_check_symbol_expressions() {
	char* expression = "(- + a)";
	Tokenizer* tokenizer = generateTokenizer(expression);

	ASSERT_TRUE(nextToken(tokenizer)->tokenType == L_PAREN);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == SYMBOL);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == SYMBOL);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == SYMBOL);
	ASSERT_TRUE(nextToken(tokenizer)->tokenType == R_PAREN);
	ASSERT_TRUE(nextToken(tokenizer) == NULL);

	return 0;
}

static void tokenizer_tests() {
	TEST(tokenizer_check_simple_expression);
	TEST(tokenizer_check_multiline_with_spaces_expression);
	TEST(tokenizer_check_multiline_expression_in_line);
	TEST(tokenizer_check_nested_expressions);
	TEST(tokenizer_check_boolean_expressions);
	TEST(tokenizer_check_symbol_expressions);
}

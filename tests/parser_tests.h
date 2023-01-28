#include <string.h>
#include <stdbool.h>

#include "test.h"
#include "../src/tokenizer.h"
#include "../src/parser.h"

static int parser_should_parse_integer() {
	char* expression = "1234";
	Ast* ast = parse(generateTokenizer(expression));
	
	ASSERT_TRUE(ast->type == ATOM);
	ASSERT_TRUE(((Atom*)ast->value)->value.integer == 1234);
	
	freeAst(ast);
	return 0;
}

static int parser_should_parse_list_of_booleans() {
	char* expression = "(#t #f)";
	Ast* ast = parse(generateTokenizer(expression));
	
	ASSERT_TRUE(ast->type == LIST);
	ASSERT_TRUE(((List*)ast->value)->quantity == 2);
	
	Atom* firstChild = ((List*)ast->value)->astChildren[0]->value;
	Atom* secondChild = ((List*)ast->value)->astChildren[1]->value;
	ASSERT_TRUE(firstChild->value.boolean == true);
	ASSERT_TRUE(secondChild->value.boolean == false);
	
	freeAst(ast);
	return 0;
}

static int parser_should_parse_two_lists() {
	char* expression = "(#t #f) (+ 2)";
	Tokenizer* tokenizer = generateTokenizer(expression);
	Ast* ast = parse(tokenizer);
	
	ASSERT_TRUE(ast->type == LIST);
	ASSERT_TRUE(((List*)ast->value)->quantity == 2);
	
	Atom* firstChild = ((List*)ast->value)->astChildren[0]->value;
	Atom* secondChild = ((List*)ast->value)->astChildren[1]->value;
	ASSERT_TRUE(firstChild->value.boolean == true);
	ASSERT_TRUE(secondChild->value.boolean == false);
	
	freeAst(ast);

	ast = parse(tokenizer);
	
	ASSERT_TRUE(ast->type == LIST);
	ASSERT_TRUE(((List*)ast->value)->quantity == 2);
	
	firstChild = ((List*)ast->value)->astChildren[0]->value;
	secondChild = ((List*)ast->value)->astChildren[1]->value;
	ASSERT_TRUE(strcmp(firstChild->value.symbol, "+") == 0);
	ASSERT_TRUE(secondChild->value.integer == 2);
	
	freeAst(ast);
	return 0;
}

static int parser_should_parse_nested_lists() {
	char* expression = "(+ (- 2) 4)";
	Ast* ast = parse(generateTokenizer(expression));
	
	ASSERT_TRUE(ast->type == LIST);
	ASSERT_TRUE(((List*)ast->value)->quantity == 3);
	
	Atom* firstChild = ((List*)ast->value)->astChildren[0]->value;
	ASSERT_TRUE(strcmp(firstChild->value.symbol, "+") == 0);

	List* secondChild = ((List*)ast->value)->astChildren[1]->value;
	ASSERT_TRUE(secondChild->quantity == 2);

	Atom* minusChild = secondChild->astChildren[0]->value;
	Atom* twoChild = secondChild->astChildren[1]->value;
	ASSERT_TRUE(strcmp(minusChild->value.symbol, "-") == 0);
	ASSERT_TRUE(twoChild->value.integer == 2);

	Atom* thirdChild = ((List*)ast->value)->astChildren[2]->value;
	ASSERT_TRUE(thirdChild->value.integer == 4);
	
	freeAst(ast);
	return 0;
}

static void parser_tests() {
	TEST(parser_should_parse_integer);
	TEST(parser_should_parse_list_of_booleans);
	TEST(parser_should_parse_two_lists);
	TEST(parser_should_parse_nested_lists);
} 

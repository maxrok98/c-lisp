/* file minunit_example.c */

#include <stdio.h>

#include "test.h"
#include "tokenizer_tests.h"

int tests_run = 0;
int tests_failed = 0;


int foo = 7;
int bar = 4;

int test_foo() {
	ASSERT_TRUE(foo == 7);
	return 0;
}

int test_bar() {
	ASSERT_TRUE(bar == 5);
	return 0;
}

void all_tests() {
	 TEST(test_foo);
	 TEST(test_bar);
}

int main(int argc, char **argv) {
	SUITE(tokenizer_tests);

	if (tests_failed != 0) {
		printf("%d OUT OF %d FAILED!\n", tests_failed, tests_run);
		return -1;
	}

	printf("ALL %d TESTS PASSED!\n", tests_run);
	return 0;
}

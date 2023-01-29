#include <stdio.h>

#include "test.h"
#include "tokenizer_tests.h"
#include "parser_tests.h"
#include "eval_tests.h"
#include "env_tests.h"
#include "gc_tests.h"

int tests_run = 0;
int tests_failed = 0;

int main(int argc, char **argv) {
	SUITE(tokenizer_tests);
	SUITE(parser_tests);
	SUITE(env_tests);
	SUITE(gc_tests);
	SUITE(eval_tests);

	if (tests_failed != 0) {
		printf("%d OUT OF %d FAILED!\n", tests_failed, tests_run);
		return -1;
	}

	printf("ALL %d TESTS PASSED!\n", tests_run);
	return 0;
}

#pragma once

#include <stdio.h>

extern int tests_run;
extern int tests_failed;

#define TEST(name)\
	tests_run++;\
	printf("%s ... ", #name);\
	if (name() == 0) printf("OK\n"); else printf("FAILED\n");

#define ASSERT_TRUE(test)\
	if (!(test)) { tests_failed++; return -1; }

#define SUITE(name)\
	printf("\nSuite: %s\n", #name);\
	name();
	

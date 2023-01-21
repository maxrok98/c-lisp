#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "std_procs.h"

Lval* addProc(Lval** lval, int quantity) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer += lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* minusProc(Lval** lval, int quantity) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer -= lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* multiplyProc(Lval** lval, int quantity) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer *= lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* divideProc(Lval** lval, int quantity) {
	assert(lval[0]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_INTEGER;
	lvalResult->value.integer = lval[0]->value.integer;
	for(int i = 1; i < quantity; i++) {
		assert(lval[i]->type == V_INTEGER);
		lvalResult->value.integer /= lval[i]->value.integer;
	}
	return lvalResult;
}

Lval* lessProc(Lval** lval, int quantity) {
	assert(quantity == 2);
	assert(lval[0]->type == V_INTEGER);
	assert(lval[1]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->value.integer < lval[1]->value.integer;
	return lvalResult;
}

Lval* moreProc(Lval** lval, int quantity) {
	assert(quantity == 2);
	assert(lval[0]->type == V_INTEGER);
	assert(lval[1]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->value.integer > lval[1]->value.integer;
	return lvalResult;
}

Lval* equalProc(Lval** lval, int quantity) {
	assert(quantity == 2);
	assert(lval[0]->type == V_INTEGER);
	assert(lval[1]->type == V_INTEGER);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->value.integer == lval[1]->value.integer;
	return lvalResult;
}

Lval* andProc(Lval** lval, int quantity) {
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	for(int i = 0; i < quantity; i++) {
		if(lval[i]->value.boolean == false) {
			lvalResult->value.boolean = false;
			return lvalResult;
		}
	}
	lvalResult->value.boolean = true;
	return lvalResult;
}

Lval* orProc(Lval** lval, int quantity) {
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	for(int i = 0; i < quantity; i++) {
		if(lval[i]->type != V_BOOLEAN || (lval[i]->type == V_BOOLEAN && lval[i]->value.boolean == true)) {
			lvalResult->value.boolean = true;
			return lvalResult;
		}
	}
	lvalResult->value.boolean = false;
	return lvalResult;
}

Lval* notProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	if(lval[0]->type == V_BOOLEAN && lval[0]->value.boolean == false) {
		lvalResult->value.boolean = true;
		return lvalResult;
	}
	lvalResult->value.boolean = false;
	return lvalResult;
}

Lval* consProc(Lval** lval, int quantity) {
	assert(quantity == 2);
	Lval* lvalResult = createLval();
	lvalResult->type = V_PAIR;
	lvalResult->value.pair = (Pair*)malloc(sizeof(Pair));
	lvalResult->value.pair->car = lval[0];
	lvalResult->value.pair->cdr = lval[1];
	
	return lvalResult;
}

Lval* carProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	assert(lval[0]->type == V_PAIR);
	
	return lval[0]->value.pair->car;
}

Lval* cdrProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	assert(lval[0]->type == V_PAIR);
	
	return lval[0]->value.pair->cdr;
}

Lval* listProc(Lval** lval, int quantity) {
	Lval* lvalNull = createLval();
	lvalNull->type = V_NULL;
	if(quantity == 0) {
		return lvalNull;
	}
	Lval* lvalResult = lvalNull;
	for(int i = quantity - 1; i >= 0; i--) {
		Lval** lvalList = (Lval**)malloc(sizeof(Lval*) * 2);
		lvalList[0] = lval[i];
		lvalList[1] = lvalResult;
		lvalResult = consProc(lvalList, 2);

		free(lvalList);
	}
	return lvalResult;
}

Lval* numberPredProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->type == V_INTEGER;

	return lvalResult;
}

Lval* booleanPredProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->type == V_BOOLEAN;

	return lvalResult;
}

Lval* pairPredProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->type == V_PAIR;

	return lvalResult;
}

Lval* nullPredProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->type == V_NULL;

	return lvalResult;
}

Lval* lambdaPredProc(Lval** lval, int quantity) {
	assert(quantity == 1);
	Lval* lvalResult = createLval();
	lvalResult->type = V_BOOLEAN;
	lvalResult->value.boolean = lval[0]->type == V_LAMBDA;

	return lvalResult;
}

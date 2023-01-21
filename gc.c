#include <stdbool.h>
#include <stdlib.h>

#include "gc.h"
#include "env.h"
#include "eval.h"

GcPool* GlobalGcPool;

GcPool* collect(GcPool* gcPool);
void freeGcPool(GcPool* gcPool);
void markEnv(Env* env);
void markLval(Lval* lval);

GcPool* getGlobalGcPool() {
	return GlobalGcPool;
}

void setGlobalGcPool(GcPool* gcPool) {
	GlobalGcPool = gcPool;
}

GcPool* createGcPool() {
	GcPool* gcPool = (GcPool*)malloc(sizeof(GcPool));
	gcPool->quantity = 0;
	gcPool->size = GC_POOL_SIZE;
	
	gcPool->gcRefs = (GcRef**)malloc(sizeof(GcRef*) * gcPool->size);
	return gcPool;
}

GcPool* garbageCollect(Env* root) {
	markEnv(root);	
	GcPool* newPool = collect(GlobalGcPool);

	freeGcPool(GlobalGcPool);
	GlobalGcPool = newPool;
	return newPool;
}

void addGcRef(GcPool* gcPool, void* ref, RefType refType) {
	if(gcPool->quantity == gcPool->size) {
		gcPool->size += gcPool->size / 2;
		gcPool->gcRefs = (GcRef**)realloc(gcPool->gcRefs, sizeof(GcRef*) * gcPool->size);
	}
	GcRef* gcRef = (GcRef*)malloc(sizeof(GcRef));
	gcRef->refType = refType;
	gcRef->reference = ref;
	gcPool->gcRefs[gcPool->quantity] = gcRef;
	gcPool->quantity++;
}

GcPool* collect(GcPool* gcPool) {
	GcPool* newPool = createGcPool();
	for(int i = 0; i < gcPool->quantity; i++) {
		if(gcPool->gcRefs[i]->refType == LVAL) {
			Lval* lval = (Lval*)gcPool->gcRefs[i]->reference;
			if(lval->gcMark) {
				addGcRef(newPool, (void*)lval, LVAL);
				lval->gcMark = false;
			}
			else {
				freeLval(lval);
			}
		}
		else if(gcPool->gcRefs[i]->refType == ENV) {
			Env* env = (Env*)gcPool->gcRefs[i]->reference;
			if(env->gcMark) {
				addGcRef(newPool, (void*)env, ENV);
				env->gcMark = false;
			}
			else {
				freeEnv(env);
			}
		}
	}
	return newPool;
}

void freeGcPool(GcPool* gcPool) {
	for(int i = 0; i < gcPool->quantity; i++) {
		free(gcPool->gcRefs[i]);
	}
	free(gcPool->gcRefs);
	free(gcPool);
}

void markEnv(Env* env) {
	if(!env->gcMark) {
		env->gcMark = true;
		for(int i = 0; i < ENV_SIZE; i++) {
			Var* varBucket = env->variables[i];
			while(varBucket != NULL) {
				markLval(varBucket->value);
				varBucket = varBucket->next;
			}
		}
		if(env->root != NULL) {
			markEnv(env->root);
		}
	}
}

void markLval(Lval* lval) {
	if(!lval->gcMark) {
		lval->gcMark = true;
		if(lval->type == V_LAMBDA) {
			Lambda* lambda = lval->value.lambda;
			if(lambda->type == CONSTRUCTED) {
				markEnv(lambda->env);
			}
		}
		else if(lval->type == V_PAIR) {
			markLval(lval->value.pair->car);
			markLval(lval->value.pair->cdr);
		}
	}
	
}

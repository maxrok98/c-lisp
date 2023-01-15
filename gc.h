#pragma once

#include "eval.h"

#define GC_POOL_SIZE 32

typedef enum RefType {
	LVAL,
	ENV
} RefType;

typedef struct gcRef {
	RefType refType;
	void* reference;
} GcRef;

typedef struct GcPool {
	GcRef** gcRefs;
	int quantity;

	int size;
} GcPool;

GcPool* createGcPool();
GcPool* getGlobalGcPool();
void setGlobalGcPool(GcPool* gcPool);
GcPool* garbageCollect(Env* root);
void addGcRef(GcPool* gcPool, void* ref, RefType refType);

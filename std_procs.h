#pragma once

#include "eval.h"

Lval* addProc(Lval** lval, int quantity);
Lval* minusProc(Lval** lval, int quantity);
Lval* multiplyProc(Lval** lval, int quantity);
Lval* divideProc(Lval** lval, int quantity);
Lval* lessProc(Lval** lval, int quantity);
Lval* moreProc(Lval** lval, int quantity);
Lval* equalProc(Lval** lval, int quantity);
Lval* andProc(Lval** lval, int quantity);
Lval* orProc(Lval** lval, int quantity);
Lval* notProc(Lval** lval, int quantity);
Lval* consProc(Lval** lval, int quantity);
Lval* carProc(Lval** lval, int quantity);
Lval* cdrProc(Lval** lval, int quantity);
Lval* listProc(Lval** lval, int quantity);
Lval* setCarProc(Lval** lval, int quantity);
Lval* setCdrProc(Lval** lval, int quantity);
Lval* numberPredProc(Lval** lval, int quantity);
Lval* booleanPredProc(Lval** lval, int quantity);
Lval* pairPredProc(Lval** lval, int quantity);
Lval* nullPredProc(Lval** lval, int quantity);
Lval* lambdaPredProc(Lval** lval, int quantity);


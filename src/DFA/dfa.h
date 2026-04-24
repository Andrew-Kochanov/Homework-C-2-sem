#pragma once
#include <stdbool.h>

// Коды ошибок для dfaAccept
typedef enum {

    // строка принадлежит языку
    DfaOk,

    // встречен символ, не входящий ни в одну категорию
    DfaInvalidSymbol,

    // нет перехода из текущего состояния по данной категории
    DfaNoTransition
} DFAError;

typedef struct {
    int from;
    char category;
    int to;
} Transition;

typedef struct {
    Transition* trans;
    int transCount;
    int* finalStates;
    int finalCount;
    int startState;
    const char** categories;
    int numCategories;
} Automat;

void automatInit(Automat* automat, Transition* trans, int transCount,
    int* finalStates, int finalCount, int startState, const char** categories,
    int numCategories);

bool dfaAccept(const Automat* automat, const char* str, DFAError* errorCode);

#pragma once
#include <stdbool.h>

// Коды ошибок для dfaAccept
typedef enum {

    // строка принадлежит языку
    DFA_OK,

    // встречен символ, не входящий ни в одну категорию
    DFA_INVALID_SYMBOL,

    // нет перехода из текущего состояния по данной категории
    DFA_NO_TRANSITION
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

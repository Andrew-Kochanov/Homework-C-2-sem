#pragma once
#include <stdbool.h>

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
} Automat;

void automatInit(Automat* automat, Transition* trans, int transCount,
    int* finals, int finalCount, int start);

bool dfaAccept(Automat* automat, char* str);

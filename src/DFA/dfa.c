#include "dfa.h"
#include <string.h>

// Вспомогательная функция: определить категорию символа
static int getCategory(const Automat* automat, char ch)
{
    for (int i = 0; i < automat->numCategories; ++i) {

        // Проверяем, входит ли символ в строку categories[i]
        if (strchr(automat->categories[i], ch) != NULL) {
            return i;
        }
    }
    return -1;
}

void automatInit(Automat* automat, Transition* trans, int transCount,
    int* final, int finalCount, int start, const char** categories,
    int numCategories)
{
    automat->trans = trans;
    automat->transCount = transCount;
    automat->finalStates = final;
    automat->finalCount = finalCount;
    automat->startState = start;
    automat->categories = categories;
    automat->numCategories = numCategories;
}

static bool isFinal(const Automat* automat, int state)
{
    for (int i = 0; i < automat->finalCount; ++i) {
        if (automat->finalStates[i] == state) {
            return true;
        }
    }
    return false;
}

static int nextState(const Automat* automat, int current, char ch, DFAError* error)
{
    int cat = getCategory(automat, ch);
    if (cat == -1) {
        if (error) {
            *error = dfaInvalidSymbol;
        }
        return -1;
    }
    for (int i = 0; i < automat->transCount; ++i) {
        if (automat->trans[i].from == current && automat->trans[i].category == cat) {
            return automat->trans[i].to;
        }
    }
    // допустимая категория, но переход не определён
    if (error) {
        *error = dfaNoTransition;
    }
    return -1;
}

bool dfaAccept(const Automat* automat, const char* str, DFAError* errorCode)
{
    if (errorCode) {
        *errorCode = dfaOk;
    }

    int state = automat->startState;
    for (const char* p = str; *p; ++p) {
        state = nextState(automat, state, *p, errorCode);
        if (state == -1) {
            return false;
        }
    }
    if (isFinal(automat, state)) {
        if (errorCode) {
            *errorCode = dfaOk;
        }

        return true;
    } else {
        if (errorCode && *errorCode == dfaOk) {
            *errorCode = dfaNoTransition;
        }

        return false;
    }
}
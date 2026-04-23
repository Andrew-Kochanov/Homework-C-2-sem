#include "dfa.h"

// Классификация символов
static int category(char c)
{
    if (c >= '0' && c <= '9')
        return 0;
    if (c == '.')
        return 1;
    if (c == 'E')
        return 2;
    if (c == '+')
        return 3;
    if (c == '-')
        return 4;

    // недопустимый символ
    return -1;
}

void automatInit(Automat* automat, Transition* trans, int transCount,
    int* finals, int finalCount, int start)
{
    automat->trans = trans;
    automat->transCount = transCount;
    automat->finalStates = finals;
    automat->finalCount = finalCount;
    automat->startState = start;
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

static int nextState(const Automat* automat, int current, char ch)
{
    int cat = category(ch);

    // Символ не из алфавита
    if (cat == -1) {
        return -1;
    }

    for (int i = 0; i < automat->transCount; ++i) {
        if (automat->trans[i].from == current && automat->trans[i].category == cat) {
            return automat->trans[i].to;
        }
    }
    // Нет перехода
    return -1;
}

bool dfaAccept(Automat* automat, const char* str)
{
    int state = automat->startState;
    for (const char* p = str; *p; ++p) {
        state = nextState(automat, state, *p);
        if (state == -1) {
            return false;
        }
    }
    return isFinal(automat, state);
}
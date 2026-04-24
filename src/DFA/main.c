#include "dfa.h"
#include <stdio.h>
#include <string.h>

// Определение категорий символов (индексы соответствуют номерам категорий)
static const char* CATEGORIES[] = { "0123456789", ".", "E", "+", "-" };
#define NUM_CATEGORIES (sizeof(CATEGORIES) / sizeof(CATEGORIES[0]))

// Категории символов
#define CAT_DIGIT 0
#define CAT_DOT 1
#define CAT_E 2
#define CAT_PLUS 3
#define CAT_MINUS 4

// Состояния автомата
#define STATE_START 0
#define STATE_SIGN 1
#define STATE_INTEGER 2
#define STATE_DOT 3
#define STATE_FRACTION 4
#define STATE_EXP 5
#define STATE_EXP_SIGN 6
#define STATE_EXP_DIGIT 7

int main(void)
{
    Transition transitions[] = {
        // Начальное состояние (STATE_START)
        { STATE_START, CAT_DIGIT, STATE_INTEGER },
        { STATE_START, CAT_DOT, STATE_DOT },
        { STATE_START, CAT_MINUS, STATE_SIGN },

        // После знака (STATE_SIGN)
        { STATE_SIGN, CAT_DIGIT, STATE_INTEGER },
        { STATE_SIGN, CAT_DOT, STATE_DOT },

        // Целая часть (STATE_INTEGER)
        { STATE_INTEGER, CAT_DIGIT, STATE_INTEGER },
        { STATE_INTEGER, CAT_DOT, STATE_DOT },
        { STATE_INTEGER, CAT_E, STATE_EXP },

        // После точки без цифр (STATE_DOT)
        { STATE_DOT, CAT_DIGIT, STATE_FRACTION },

        // Дробная часть (STATE_FRACTION)
        { STATE_FRACTION, CAT_DIGIT, STATE_FRACTION },
        { STATE_FRACTION, CAT_E, STATE_EXP },

        // После E (STATE_EXP)
        { STATE_EXP, CAT_DIGIT, STATE_EXP_DIGIT },
        { STATE_EXP, CAT_PLUS, STATE_EXP_SIGN },
        { STATE_EXP, CAT_MINUS, STATE_EXP_SIGN },

        // После знака экспоненты (STATE_EXP_SIGN)
        { STATE_EXP_SIGN, CAT_DIGIT, STATE_EXP_DIGIT },

        // Цифры экспоненты (STATE_EXP_DIGIT)
        { STATE_EXP_DIGIT, CAT_DIGIT, STATE_EXP_DIGIT }
    };
    int transCount = sizeof(transitions) / sizeof(Transition);

    int finalStates[] = { STATE_INTEGER, STATE_FRACTION, STATE_EXP_DIGIT };
    int finalCount = 3;

    Automat automat;
    automatInit(&automat, transitions, transCount, finalStates, finalCount,
        STATE_START, CATEGORIES, NUM_CATEGORIES);

    char input[256];
    printf("Введите строку для проверки: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    DFAError err = dfaAccept(&automat, input)
    if (err == DfaOk) {
        printf("Это число!\n");
    } else {
        printf("Это не число:(\n");
    }
    return 0;
}
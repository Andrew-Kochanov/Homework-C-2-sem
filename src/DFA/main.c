#include "dfa.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    // категории: 0-цифра,1-точка,2-E,3-+,4--
    Transition transitions[] = {
        // из 0 (начальное)
        { 0, 0, 2 }, { 0, 1, 3 }, { 0, 4, 1 },
        // из 1 (после знака)
        { 1, 0, 2 }, { 1, 1, 3 },
        // из 2 (целая часть)
        { 2, 0, 2 }, { 2, 1, 3 }, { 2, 2, 5 },
        // из 3 (точка без дробной части)
        { 3, 0, 4 },
        // из 4 (дробная часть)
        { 4, 0, 4 }, { 4, 2, 5 },
        // из 5 (E)
        { 5, 0, 7 }, { 5, 3, 6 }, { 5, 4, 6 },
        // из 6 (знак после E)
        { 6, 0, 7 },
        // из 7 (цифры экспоненты)
        { 7, 0, 7 }
    };
    int transCount = sizeof(transitions) / sizeof(Transition);

    int finalStates[] = { 2, 4, 7 };
    int finalCount = 3;

    Automat automat;
    automatInit(&automat, transitions, transCount,
        finalStates, finalCount, 0);

    char input[256];
    printf("Введите строку для проверки: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (dfaAccept(&automat, input)) {
        printf("Это число!\n");
    } else {
        printf("Это не число:(\n");
    }
    return 0;
}
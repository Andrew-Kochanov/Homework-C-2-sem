#include "dfa.h"
#include <assert.h>
#include <stdio.h>

void run_tests()
{
    Transition transitions[] = {
        { 0, 0, 2 }, { 0, 1, 3 }, { 0, 4, 1 },
        { 1, 0, 2 }, { 1, 1, 3 },
        { 2, 0, 2 }, { 2, 1, 3 }, { 2, 2, 5 },
        { 3, 0, 4 },
        { 4, 0, 4 }, { 4, 2, 5 },
        { 5, 0, 7 }, { 5, 3, 6 }, { 5, 4, 6 },
        { 6, 0, 7 },
        { 7, 0, 7 }
    };
    int transCount = sizeof(transitions) / sizeof(Transition);
    int finalStates[] = { 2, 4, 7 };
    int finalCount = 3;
    Automat automat;
    automatInit(&automat, transitions, transCount, finalStates, finalCount, 0);

    // ---- Тесты из условия ----
    assert(dfaAccept(&automat, "38.871E5") == true);
    assert(dfaAccept(&automat, ".591") == true);
    assert(dfaAccept(&automat, "А я число?") == false);
    assert(dfaAccept(&automat, "823.16.10") == false);
    assert(dfaAccept(&automat, "-38.871E5") == true);
    assert(dfaAccept(&automat, "-1") == true);
    assert(dfaAccept(&automat, "12E") == false);
    assert(dfaAccept(&automat, "-.25") == true);
    assert(dfaAccept(&automat, ".") == false);
    assert(dfaAccept(&automat, "-.") == false);

    printf("All tests are passed\n");
}

int main(void)
{
    run_tests();
    return 0;
}
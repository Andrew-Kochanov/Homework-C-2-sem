#include "CSVPP.h"
#include <ctype.h>

int main(void)
{
    FILE* in = fopen("input.csv", "r");
    Table table = { 0 };
    readCSV(in, &table);
    fclose(in);
    FILE* out = fopen("output.txt", "w");
    printTableToFile(&table, out);
    fclose(out);
    freeTable(&table);
    return 0;
}
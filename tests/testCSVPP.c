#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVPP.h"

// Вспомогательная функция для сравнения двух файлов
static int filesEqual(const char* f1, const char* f2) {
    FILE* a = fopen(f1, "r");
    FILE* b = fopen(f2, "r");
    if (!a || !b) {
        if (a) fclose(a);
        if (b) fclose(b);
        return 0;
    }
    char ca, cb;
    int equal = 1;
    while (equal && (ca = fgetc(a)) != EOF && (cb = fgetc(b)) != EOF) {
        if (ca != cb) equal = 0;
    }
    if (fgetc(a) != fgetc(b)) equal = 0;
    fclose(a);
    fclose(b);
    return equal;
}


void test_read_and_print() {
    // Создаём CSV
    FILE* in = fopen("test_input.csv", "w");
    assert(in);
    fprintf(in, "Name,Age,Height\n");
    fprintf(in, "Alice,25,150\n");
    fprintf(in, "Bob,thirty,150.1\n");
    fclose(in);

    Table t = {0};
    assert(readCSV("test_input.csv", &t));
    assert(t.rows == 3);
    assert(t.cols == 3);
    // Проверка на число
    assert(t.data[0][0]->isNumeric == 0); // "Name" – текст
    assert(t.data[0][1]->isNumeric == 0); // "Age" – текст
    assert(t.data[0][2]->isNumeric == 0); // "Height" – текст
    assert(t.data[1][0]->isNumeric == 0); // "Alice" – текст
    assert(t.data[1][1]->isNumeric == 1); // "25" – число
    assert(t.data[1][2]->isNumeric == 1); // "150" – число
    assert(t.data[2][0]->isNumeric == 0); // "Bob" - текст
    assert(t.data[2][1]->isNumeric == 0); // "thirty" – не число
    assert(t.data[2][2]->isNumeric == 1); // "150.1" - число

    assert(t.colWidths[0] >= 6);
    assert(t.colWidths[1] >= 5);
    assert(t.colWidths[2] >= 5);

    // Вывод в файл
    assert(printTableToFile(&t, "test_output.txt"));
    // Ожидаемый вывод
    const char* expected = 
        "+========+=====+=======+\n"
        "| Name   | Age | Score |\n"
        "+========+=====+=======+\n"
        "| Alice  |  25 |  98.5 |\n"
        "+--------+-----+-------+\n"
        "| Bob    |thirty|   100 |\n"
        "+--------+-----+-------+\n";
    // Запишем его в файл
    FILE* exp = fopen("expected.txt", "w");
    assert(exp);
    fputs(expected, exp);
    fclose(exp);
    assert(filesEqual("test_output.txt", "expected.txt"));

    freeTable(&t);
    remove("test_input.csv");
    remove("test_output.txt");
    remove("expected.txt");
}

// пустой CSV
void testEmptyCsv() {
    FILE* in = fopen("empty.csv", "w");
    fclose(in);
    Table t = {0};
    assert(readCSV("empty.csv", &t) == false);
    freeTable(&t);
    remove("empty.csv");
}

// несуществующий файл
void testMissingFile() {
    Table t = {0};
    assert(readCSV("nonexistent.csv", &t) == false);
    freeTable(&t);
}

int main(void) {
    test_read_and_print();
    test_empty_csv();
    test_missing_file();
    printf("All tests passed!\n");
    return 0;
}
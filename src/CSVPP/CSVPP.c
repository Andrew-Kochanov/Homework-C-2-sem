#include "CSVPP.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Проверка строки на число
static bool isNumericString(char* str)
{
    char* endptr;
    (void)strtod(str, &endptr);
    if (endptr == str) {
        return false;
    }
    while (*endptr && isspace((unsigned char)*endptr)) {
        ++endptr;
    }
    return *endptr == '\0';
}

// Очистка памяти таблицы
void freeTable(Table* table)
{
    if (!table)
        return;
    for (int i = 0; i < table->rows; ++i) {
        for (int j = 0; j < table->cols; ++j) {
            free(table->data[i][j].value);
        }
        free(table->data[i]);
    }
    free(table->data);
    free(table->colWidths);
    table->data = NULL;
    table->rows = 0;
    table->cols = 0;
    table->colWidths = NULL;
}

bool readCSV(FILE* in, Table* table)
{

    // Чтение всех строк файла
    char** lines = NULL;
    int numLines = 0;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), in)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        lines = realloc(lines, (numLines + 1) * sizeof(char*));
        lines[numLines] = malloc(strlen(buffer) + 1);
        strcpy(lines[numLines], buffer);
        ++numLines;
    }

    if (numLines == 0) {
        free(lines);
        return false;
    }

    // Количество столбцов по первой строке
    int cols = 1;
    char* p = lines[0];
    while (*p != '\0') {
        if (*p == ',') {
            ++cols;
        }
        p++;
    }

    // Выделение памяти под таблицу
    Cell** data = (Cell**)malloc(numLines * sizeof(Cell*));
    for (int i = 0; i < numLines; ++i) {
        data[i] = (Cell*)malloc(cols * sizeof(Cell));
        for (int j = 0; j < cols; ++j) {
            data[i][j].value = NULL;
            data[i][j].isNumeric = false;
        }
    }

    // Разбор строк на поля
    for (int i = 0; i < numLines; ++i) {
        char* saveptr;
        char* token = strtok_r(lines[i], ",", &saveptr);
        for (int j = 0; j < cols; ++j) {
            data[i][j].value = malloc(strlen(token) + 1);
            strcpy(data[i][j].value, token);
            data[i][j].isNumeric = isNumericString(token);
            token = strtok_r(NULL, ",", &saveptr);
        }
    }

    // Вычисление ширины столбцов
    int* colWidths = calloc(cols, sizeof(int));
    for (int j = 0; j < cols; ++j) {
        int maxW = 0;
        for (int i = 0; i < numLines; ++i) {
            int len = strlen(data[i][j].value);
            if (len > maxW) {
                maxW = len;
            }
        }
        colWidths[j] = maxW;
    }

    table->rows = numLines;
    table->cols = cols;
    table->colWidths = colWidths;
    table->data = (Cell**)data;

    for (int i = 0; i < numLines; ++i) {
        free(lines[i]);
    }
    free(lines);

    return true;
}
// Функция печати горизонтальной линии
static void printLine(char ch, FILE* out, int cols, int* colWidths)
{
    fputc('+', out);
    for (int j = 0; j < cols; ++j) {
        for (int k = 0; k < colWidths[j] + 2; ++k)
            fputc(ch, out);
        if (j < cols - 1) {
            fputc('+', out);
        }
    }
    fputc('+', out);
    fputc('\n', out);
}

// Печать таблицы в файл
void printTableToFile(const Table* table, FILE* out)
{

    if (!table || !table->data || table->rows == 0) {
        return;
    }

    // Верхняя граница
    printLine('=', out, table->cols, table->colWidths);

    // Заголовок (первая строка)
    fputc('|', out);
    for (int j = 0; j < table->cols; ++j) {
        fprintf(out, " %-*s |", table->colWidths[j], table->data[0][j].value);
    }
    fputc('\n', out);

    // Разделитель после заголовка
    printLine('=', out, table->cols, table->colWidths);

    // Строки данных (начиная со второй)
    for (int i = 1; i < table->rows; ++i) {
        fputc('|', out);
        for (int j = 0; j < table->cols; ++j) {
            if (table->data[i][j].isNumeric) {
                fprintf(out, " %*s |", table->colWidths[j], table->data[i][j].value);
            } else {
                fprintf(out, " %-*s |", table->colWidths[j], table->data[i][j].value);
            }
        }
        fputc('\n', out);
        printLine('-', out, table->cols, table->colWidths);
    }
}

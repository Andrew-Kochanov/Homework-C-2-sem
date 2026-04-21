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
        return 0;
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
            Cell* cell = (Cell*)table->data[i][j];
            if (cell) {
                free(cell->value);
                free(cell);
            }
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

bool readCSV(const char* filename, Table* table)
{

    FILE* in = fopen("input.csv", "r");
    if (!in) {
        printf("Не удалось открыть входной файл\n");
        return 1;
    }

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
    fclose(in);

    if (numLines == 0) {
        free(lines);
        return 0;
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
    Cell*** data = malloc(numLines * sizeof(Cell**));
    for (int i = 0; i < numLines; ++i) {
        data[i] = malloc(cols * sizeof(Cell*));
        for (int j = 0; j < cols; ++j) {
            data[i][j] = malloc(sizeof(Cell));
            data[i][j]->value = NULL;
            data[i][j]->isNumeric = 0;
        }
    }

    // Разбор строк на поля
    for (int i = 0; i < numLines; ++i) {
        char* saveptr;
        char* token = strtok_r(lines[i], ",", &saveptr);
        for (int j = 0; j < cols; ++j) {
            data[i][j]->value = malloc(strlen(token) + 1);
            strcpy(data[i][j]->value, token);
            data[i][j]->isNumeric = isNumericString(token);
            token = strtok_r(NULL, ",", &saveptr);
        }
    }

    // Вычисление ширины столбцов
    int* colWidths = calloc(cols, sizeof(int));
    for (int j = 0; j < cols; ++j) {
        int max_w = 0;
        for (int i = 0; i < numLines; ++i) {
            int len = strlen(data[i][j]->value);
            if (len > max_w) {
                max_w = len;
            }
        }
        colWidths[j] = max_w;
    }

    table->rows = numLines;
    table->cols = cols;
    table->colWidths = colWidths;
    table->data = (Cell***)data;

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
bool printTableToFile(const Table* table, const char* filename)
{
    if (!table || !table->data || table->rows == 0 || table->cols == 0) {
        return false;
    }

    // Открытие выходного файла
    FILE* out = fopen(filename, "w");
    if (!out) {
        printf("Не удалось открыть выходной файл\n");
        return false;
    }

    Cell*** data = (Cell***)table->data;
    // Верхняя граница
    printLine('=', out, table->cols, table->colWidths);

    // Заголовок (первая строка)
    fputc('|', out);
    for (int j = 0; j < table->cols; ++j) {
        fprintf(out, " %-*s |", table->colWidths[j], data[0][j]->value);
    }
    fputc('\n', out);

    // Разделитель после заголовка
    printLine('=', out, table->cols, table->colWidths);

    // Строки данных (начиная со второй)
    for (int i = 1; i < table->rows; ++i) {
        fputc('|', out);
        for (int j = 0; j < table->cols; ++j) {
            if (data[i][j]->isNumeric) {
                fprintf(out, " %*s |", table->colWidths[j], data[i][j]->value);
            } else {
                fprintf(out, " %-*s |", table->colWidths[j], data[i][j]->value);
            }
        }
        fputc('\n', out);
        printLine('-', out, table->cols, table->colWidths);
    }

    fclose(out);
    return true;
}

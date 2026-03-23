#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* value;
    int isNumeric;
} Cell;

int isNumericString(char* str)
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

// Функция печати горизонтальной линии
void printLine(char ch, FILE* out, int cols, int* colWidths)
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

int main(void)
{
    int maxLine = 4096;

    FILE* in = fopen("input.csv", "r");

    // Чтение всех строк файла
    char** lines = NULL;
    int numLines = 0;
    char buffer[maxLine];
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
    for (char* p = lines[0]; *p; ++p)
        if (*p == ',') {
            ++cols;
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

    // Открытие выходного файла
    FILE* out = fopen("output.txt", "w");

    // Верхняя граница
    printLine('=', out, cols, colWidths);

    // Заголовок (первая строка)
    fputc('|', out);
    for (int j = 0; j < cols; ++j) {
        fprintf(out, " %-*s |", colWidths[j], data[0][j]->value);
    }
    fputc('\n', out);

    // Разделитель после заголовка
    printLine('=', out, cols, colWidths);

    // Строки данных (начиная со второй)
    for (int i = 1; i < numLines; ++i) {
        fputc('|', out);
        for (int j = 0; j < cols; ++j) {
            if (data[i][j]->isNumeric) {
                fprintf(out, " %*s |", colWidths[j], data[i][j]->value);
            } else {
                fprintf(out, " %-*s |", colWidths[j], data[i][j]->value);
            }
        }
        fputc('\n', out);
        printLine('-', out, cols, colWidths);
    }

    fclose(out);

    // Освобождение памяти
    for (int i = 0; i < numLines; ++i) {
        for (int j = 0; j < cols; ++j) {
            free(data[i][j]->value);
            free(data[i][j]);
        }
        free(data[i]);
        free(lines[i]);
    }
    free(data);
    free(lines);
    free(colWidths);

    return 0;
}
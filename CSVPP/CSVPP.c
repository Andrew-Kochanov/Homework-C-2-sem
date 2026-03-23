#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

typedef struct {
    char* value;
    int is_numeric;
} Cell;

int is_numeric_string(const char* str)
{
    char* endptr;
    (void)strtod(str, &endptr);
    if (endptr == str)
        return 0;
    while (*endptr && isspace((unsigned char)*endptr)) {
        ++endptr
    };
    return *endptr == '\0';
}

int main(void)
{
    FILE* in = fopen("input.csv", "r");

    // Чтение всех строк файла
    char** lines = NULL;
    int num_lines = 0;
    char buffer[MAX_LINE];
    while (fgets(buffer, sizeof(buffer), in)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        lines = realloc(lines, (num_lines + 1) * sizeof(char*));
        lines[num_lines] = malloc(strlen(buffer) + 1);
        strcpy(lines[num_lines], buffer);
        ++num_lines;
    }
    fclose(in);

    if (num_lines == 0) {
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
    Cell*** data = malloc(num_lines * sizeof(Cell**));
    for (int i = 0; i < num_lines; ++i) {
        data[i] = malloc(cols * sizeof(Cell*));
        for (int j = 0; j < cols; ++j) {
            data[i][j] = malloc(sizeof(Cell));
            data[i][j]->value = NULL;
            data[i][j]->is_numeric = 0;
        }
    }

    // Разбор строк на поля
    for (int i = 0; i < num_lines; ++i) {
        char* saveptr;
        char* token = strtok_r(lines[i], ",", &saveptr);
        for (int j = 0; j < cols; ++j) {
            data[i][j]->value = malloc(strlen(token) + 1);
            strcpy(data[i][j]->value, token);
            data[i][j]->is_numeric = is_numeric_string(token);
            token = strtok_r(NULL, ",", &saveptr);
        }
    }

    // Вычисление ширины столбцов
    int* col_widths = calloc(cols, sizeof(int));
    for (int j = 0; j < cols; ++j) {
        int max_w = 0;
        for (int i = 0; i < num_lines; ++i) {
            int len = strlen(data[i][j]->value);
            if (len > max_w)
                max_w = len;
        }
        col_widths[j] = max_w;
    }

    // Открытие выходного файла
    FILE* out = fopen("output.txt", "w");

    // Функция печати горизонтальной линии
    void print_line(char ch)
    {
        fputc('+', out);
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < col_widths[j] + 2; ++k)
                fputc(ch, out);
            if (j < cols - 1)
                fputc('+', out);
        }
        fputc('+', out);
        fputc('\n', out);
    }

    // Верхняя граница
    print_line('=');

    // Заголовок (первая строка)
    fputc('|', out);
    for (int j = 0; j < cols; ++j) {
        fprintf(out, " %-*s |", col_widths[j], data[0][j]->value);
    }
    fputc('\n', out);

    // Разделитель после заголовка
    print_line('=');

    // Строки данных (начиная со второй)
    for (int i = 1; i < num_lines; ++i) {
        fputc('|', out);
        for (int j = 0; j < cols; ++j) {
            if (data[i][j]->is_numeric) {
                fprintf(out, " %*s |", col_widths[j], data[i][j]->value);
            } else {
                fprintf(out, " %-*s |", col_widths[j], data[i][j]->value);
            }
        }
        fputc('\n', out);
        print_line('-');
    }

    fclose(out);

    // Освобождение памяти
    for (int i = 0; i < num_lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            free(data[i][j]->value);
            free(data[i][j]);
        }
        free(data[i]);
        free(lines[i]);
    }
    free(data);
    free(lines);
    free(col_widths);

    return 0;
}
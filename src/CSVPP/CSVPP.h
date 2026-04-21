#pragma once
#include <stdbool.h>
#include <stdio.h>

// Ячейка таблицы
typedef struct {
    char* value;
    bool isNumeric;
} Cell;

// Таблица
typedef struct {
    Cell** data;
    int rows;
    int cols;
    int* colWidths;
} Table;

// Чтение CSV-файла и заполнение Table
bool readCSV(FILE* file, Table* table);

// Печать таблицы в текстовый файл
void printTableToFile(const Table* table, FILE* file);

// Освобождение памяти таблицы
void freeTable(Table* table);
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
    Cell*** data;
    int rows;
    int cols;
    int* colWidths;
} Table;

// Чтение CSV-файла и заполнение Table
bool readCSV(const char* filename, Table* table);

// Печать таблицы в текстовый файл
bool printTableToFile(const Table* table, const char* filename);

// Освобождение памяти таблицы
void freeTable(Table* table);
#pragma once

// Распределения городов между государствами
int** solveCountries(int n, int m, int* roads, int k, int* capitals, int** sizes);

// Освобождения памяти, выделенной для результата
void freeCountriesResult(int** result, int* sizes, int k);

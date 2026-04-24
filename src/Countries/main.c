#include "countries.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Открытие входного файла
    FILE* file = fopen("data.txt", "r");
    if (!file) {
        printf("Не удалось открыть data.txt\n");
        return 1;
    }

    // Чтение числа городов и дорог
    int n, m;
    fscanf(file, "%d %d", &n, &m);

    // Чтение всех дорог
    int* roads = malloc(3 * m * sizeof(int));
    if (!roads) {
        fclose(file);
        printf("Ошибка выделения памяти для дорог\n");
        return 1;
    }
    for (int i = 0; i < m; ++i) {
        fscanf(file, "%d %d %d", &roads[3 * i], &roads[3 * i + 1], &roads[3 * i + 2]);
    }

    // Чтение числа государств и столиц
    int k;
    fscanf(file, "%d", &k);
    int* capitals = malloc(k * sizeof(int));
    if (!capitals) {
        free(roads);
        fclose(file);
        printf("Ошибка выделения памяти для столиц\n");
        return 1;
    }
    for (int i = 0; i < k; ++i) {
        fscanf(file, "%d", &capitals[i]);
    }
    fclose(file);

    // Вызов основного алгоритма
    int* sizes = NULL;
    int** result = solveCountries(n, m, roads, k, capitals, &sizes);

    // Вывод результата
    for (int g = 1; g <= k; ++g) {
        printf("Государство %d:", g);
        for (int i = 0; i < sizes[g]; ++i) {
            printf(" %d", result[g][i]);
        }
        printf("\n");
    }

    freeCountriesResult(result, sizes, k);
    free(roads);
    free(capitals);

    return 0;
}
#include "countries.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Ребро графа (список смежности)
typedef struct Edge {
    int to;
    int len;
    struct Edge* next;
} Edge;

// Узел кучи
typedef struct {
    int dist;
    int city;
} HeapNode;

// Куча
typedef struct {
    HeapNode* arr;
    int size;
    int cap;
} Heap;

// Инициализация кучи
static void heapInit(Heap* heap)
{
    heap->arr = NULL;
    heap->size = 0;
    heap->cap = 0;
}

// Добавление элемента в кучу
static bool heapPush(Heap* heap, int dist, int city)
{
    if (heap->size == heap->cap) {
        int newCap = (heap->cap == 0) ? 4 : heap->cap * 2;
        HeapNode* newArr = realloc(heap->arr, newCap * sizeof(HeapNode));
        if (!newArr)
            return false;
        heap->arr = newArr;
        heap->cap = newCap;
    }
    int i = heap->size++;
    heap->arr[i].dist = dist;
    heap->arr[i].city = city;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->arr[parent].dist <= heap->arr[i].dist)
            break;
        HeapNode tmp = heap->arr[parent];
        heap->arr[parent] = heap->arr[i];
        heap->arr[i] = tmp;
        i = parent;
    }
    return true;
}

// Извлечение минимального элемента из кучи
static bool heapPop(Heap* heap, HeapNode* result)
{
    if (heap->size == 0)
        return false;
    *result = heap->arr[0];
    heap->arr[0] = heap->arr[--heap->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist)
            smallest = left;
        if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist)
            smallest = right;
        if (smallest == i)
            break;
        HeapNode tmp = heap->arr[i];
        heap->arr[i] = heap->arr[smallest];
        heap->arr[smallest] = tmp;
        i = smallest;
    }
    return true;
}

// Освобождение памяти кучи
static void heapFree(Heap* heap)
{
    free(heap->arr);
    heap->arr = NULL;
    heap->size = 0;
    heap->cap = 0;
}

// Освобождение памяти графа (списков смежности)
static void freeGraph(int n, Edge** adj)
{
    if (!adj)
        return;
    for (int i = 1; i <= n; ++i) {
        Edge* edge = adj[i];
        while (edge) {
            Edge* tmp = edge;
            edge = edge->next;
            free(tmp);
        }
    }
    free(adj);
}

// Распределения городов между государствами
int** solveCountries(int n, int m, int* roads, int k, int* capitals, int** sizes)
{
    *sizes = NULL;

    // Построение неориентированного графа
    Edge** adj = calloc(n + 1, sizeof(Edge*));

    for (int i = 0; i < m; ++i) {
        int u = roads[3 * i];
        int v = roads[3 * i + 1];
        int len = roads[3 * i + 2];
        Edge* edge1 = malloc(sizeof(Edge));
        edge1->to = v;
        edge1->len = len;
        edge1->next = adj[u];
        adj[u] = edge1;
        Edge* edge2 = malloc(sizeof(Edge));
        edge2->to = u;
        edge2->len = len;
        edge2->next = adj[v];
        adj[v] = edge2;
    }

    // Состояние каждого города (0 – свободен, иначе номер государства)
    int* state = calloc(n + 1, sizeof(int));

    // Списки городов для каждого государства и их размеры
    int** cities = malloc((k + 1) * sizeof(int*));
    int* citiesSize = calloc(k + 1, sizeof(int));

    // Кучи для каждого государства
    Heap* heaps = malloc((k + 1) * sizeof(Heap));
    for (int g = 1; g <= k; ++g) {
        heapInit(&heaps[g]);
    }

    int totalOccupied = 0;

    // Инициализация столицами (заняты, добавлены в списки, соседи в кучи)
    for (int g = 1; g <= k; ++g) {
        int cap = capitals[g - 1];
        state[cap] = g;
        cities[g] = malloc(sizeof(int));
        cities[g][0] = cap;
        citiesSize[g] = 1;
        totalOccupied++;
        for (Edge* edge = adj[cap]; edge; edge = edge->next) {
            if (state[edge->to] == 0) {
                heapPush(&heaps[g], edge->len, edge->to);
            }
        }
    }

    // Цикл распределения
    while (totalOccupied < n) {

        // Работаем с государствами поочереди
        for (int g = 1; g <= k; ++g) {

            // Нет доступных соседей
            if (heaps[g].size == 0)
                continue;
            while (heaps[g].size > 0) {
                HeapNode best;
                heapPop(&heaps[g], &best);
                if (state[best.city] == 0) {

                    // Берем ближайший свободный город
                    state[best.city] = g;
                    int* newCities = realloc(cities[g], (citiesSize[g] + 1) * sizeof(int));
                    cities[g] = newCities;
                    cities[g][citiesSize[g]++] = best.city;
                    totalOccupied++;

                    // Добавляем соседей нового города в кучу
                    for (Edge* edge = adj[best.city]; edge; edge = edge->next) {
                        if (state[edge->to] == 0) {
                            heapPush(&heaps[g], edge->len, edge->to);
                        }
                    }

                    // Переходим к следующему государству
                    break;
                }
            }
        }
    }

    // Успех: освобождаем вспомогательные структуру
    freeGraph(n, adj);
    free(state);
    for (int g = 1; g <= k; ++g) {
        heapFree(&heaps[g]);
    }
    free(heaps);

    // Формируем результат
    int** result = malloc((k + 1) * sizeof(int*));
    int* resultSizes = malloc((k + 1) * sizeof(int));
    for (int g = 1; g <= k; ++g) {
        resultSizes[g] = citiesSize[g];
        result[g] = malloc(citiesSize[g] * sizeof(int));
        for (int i = 0; i < citiesSize[g]; ++i) {
            result[g][i] = cities[g][i];
        }
        free(cities[g]);
    }
    free(cities);
    free(citiesSize);
    *sizes = resultSizes;
    return result;
}

// Освобождения памяти, выделенной для результата
void freeCountriesResult(int** result, int* sizes, int k)
{
    if (result) {
        for (int g = 1; g <= k; ++g) {
            free(result[g]);
        }
        free(result);
    }
    free(sizes);
}
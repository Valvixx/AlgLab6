#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <random>
#include <time.h>
#include <stdio.h>

int** createG(int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            G[i][j] = rand() % 2;
            if (i == j) G[i][j] = 0;
            G[j][i] = G[i][j];
        }
    }
    return G;
}

void printG(int** G, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Преобразование матрицы смежности в список смежности
void adjMatrixToList(int** G, int size, int** adjList, int* degrees) {
    for (int i = 0; i < size; i++) {
        degrees[i] = 0; // Считаем степень вершины
        for (int j = 0; j < size; j++) {
            if (G[i][j] == 1) {
                adjList[i][degrees[i]] = j;
                degrees[i]++;
            }
        }
    }
}

// Вывод списка смежности
void printAdjList(int** adjList, int* degrees, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d: ", i);
        for (int j = 0; j < degrees[i]; j++) {
            printf("%d ", adjList[i][j]);
        }
        printf("\n");
    }
}

// Удаление вершины
int** delG(int** G, int size, int v) {
    int** G1 = (int**)malloc((size - 1) * sizeof(int*));
    for (int i = 0; i < size - 1; i++) {
        G1[i] = (int*)malloc((size - 1) * sizeof(int));
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i < v && j < v)
                G1[i][j] = G[i][j];
            else if (i < v && j > v)
                G1[i][j - 1] = G[i][j];
            else if (i > v && j < v)
                G1[i - 1][j] = G[i][j];
            else if (i > v && j > v)
                G1[i - 1][j - 1] = G[i][j];
        }
    }
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }
    free(G);
    return G1;
}

// Отождествление вершин
int** unionV(int** G, int size, int v1, int v2) {
    for (int i = 0; i < size; i++) {
        if (G[v2][i] == 1) {
            G[v1][i] = 1;
            G[i][v1] = 1;
        }
    }
    return delG(G, size, v2);
}

// Стягивание ребра
int** contrV(int** G, int size, int v1, int v2) {
    G[v1][v2] = 0;
    G[v2][v1] = 0;
    for (int i = 0; i < size; i++) {
        if (G[v2][i] == 1) {
            G[v1][i] = 1;
            G[i][v1] = 1;
        }
    }
    return delG(G, size, v2);
}

// Расщепление вершины
int** splitV(int** G, int size, int v) {
    int** Gtemp = (int**)malloc((size + 1) * sizeof(int*));
    for (int i = 0; i < size + 1; i++) {
        Gtemp[i] = (int*)malloc((size + 1) * sizeof(int));
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Gtemp[i][j] = G[i][j];
        }
    }
    for (int i = 0; i < size; i++) {
        Gtemp[i][size] = G[i][v];
        Gtemp[size][i] = G[v][i];
    }
    Gtemp[size][size] = 0;
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }
    free(G);
    return Gtemp;
}

int main() {
    srand(time(NULL));
    int nG1, nG2, v1, v2;

    printf("nG1 = ");
    scanf("%d", &nG1);

    int** G1 = createG(nG1);

    printf("Matrix G1:\n");
    printG(G1, nG1);

    printf("Ver1 for union = ");
    scanf("%d", &v1);
    printf("Ver2 for union = ");
    scanf("%d", &v2);

    G1 = unionV(G1, nG1, v1, v2);
    nG1--;

    printf("\nAfter union:\n");
    printG(G1, nG1);

    printf("Ver1 for contraction = ");
    scanf("%d", &v1);
    printf("Ver2 for contraction = ");
    scanf("%d", &v2);

    G1 = contrV(G1, nG1, v1, v2);
    nG1--;

    printf("\nAfter contraction:\n");
    printG(G1, nG1);

    printf("Ver for splitting = ");
    scanf("%d", &v1);

    G1 = splitV(G1, nG1, v1);
    nG1++;

    printf("\nAfter splitting:\n");
    printG(G1, nG1);

    for (int i = 0; i < nG1; i++) {
        free(G1[i]);
    }
    free(G1);

    return 0;
}

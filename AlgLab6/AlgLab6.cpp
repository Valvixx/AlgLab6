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

void adjMatrixToList(int** G, int size, int** adjList, int* degrees) {
    for (int i = 0; i < size; i++) {
        degrees[i] = 0;
        for (int j = 0; j < size; j++) {
            if (G[i][j] == 1) {
                adjList[i][degrees[i]] = j;
                degrees[i]++;
            }
        }
    }
}

void printAdjList(int** adjList, int* degrees, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d: ", i);
        for (int j = 0; j < degrees[i]; j++) {
            printf("%d ", adjList[i][j]);
        }
        printf("\n");
    }
}

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

int** unionV(int** G, int size, int v1, int v2) {
    for (int i = 0; i < size; i++) {
        if (G[v2][i] == 1) {
            G[v1][i] = 1;
            G[i][v1] = 1;
        }
    }
    return delG(G, size, v2);
}

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

int** unionG(int** G1, int** G2, int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            G[i][j] = G1[i][j] || G2[i][j];
        }
    }
    return G;
}

int** intersectG(int** G1, int** G2, int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            G[i][j] = G1[i][j] && G2[i][j];
        }
    }
    return G;
}

int** ringSumG(int** G1, int** G2, int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            G[i][j] = G1[i][j] ^ G2[i][j]; //XOR
        }
    }
    return G;
}


int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    int nG1, nG2, v1, v2;

    printf("Введите количество вершин для графа G1: ");
    scanf("%d", &nG1);
    int** G1 = createG(nG1);
    printf("\nМатрица смежности для графа G1:\n");
    printG(G1, nG1);

    printf("Введите количество вершин для графа G2: ");
    scanf("%d", &nG2);
    int** G2 = createG(nG2);
    printf("\nМатрица смежности для графа G2:\n");
    printG(G2, nG2);

    int** G_union = unionG(G1, G2, nG1);
    printf("\nМатрица смежности для объединения G1 и G2:\n");
    printG(G_union, nG1);

    int** G_intersect = intersectG(G1, G2, nG1);
    printf("\nМатрица смежности для пересечения G1 и G2:\n");
    printG(G_intersect, nG1);

    int** G_ringSum = ringSumG(G1, G2, nG1);
    printf("\nМатрица смежности для кольцевой суммы G1 и G2:\n");
    printG(G_ringSum, nG1);

    printf("Введите вершины для отождествления в G1 (v1 и v2): ");
    scanf("%d %d", &v1, &v2);
    G1 = unionV(G1, nG1, v1, v2);
    nG1--;
    printf("\nПосле отождествления вершин в G1:\n");
    printG(G1, nG1);

    printf("Введите вершины для стягивания ребра в G1 (v1 и v2): ");
    scanf("%d %d", &v1, &v2);
    G1 = contrV(G1, nG1, v1, v2);
    nG1--;
    printf("\nПосле стягивания ребра в G1:\n");
    printG(G1, nG1);

    printf("Введите вершину для расщепления в G1: ");
    scanf("%d", &v1);
    G1 = splitV(G1, nG1, v1);
    nG1++;
    printf("\nПосле расщепления вершины в G1:\n");
    printG(G1, nG1);


    for (int i = 0; i < nG1; i++) {
        free(G1[i]);
        free(G_intersect[i]);
        free(G_ringSum[i]);
        free(G_union[i]);
    }
    free(G1);
    for (int i = 0; i < nG2; i++) {
        free(G2[i]);
    }
    free(G_intersect);
    free(G_ringSum);
    free(G_union);
    free(G2);
    return 0;
}

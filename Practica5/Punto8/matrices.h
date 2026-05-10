#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>
#include <stdlib.h>

#define SIZEM 10


// ─────────────────────────────────────────────
void parsearMatriz(char *argv[], int startIdx, int matrix[][SIZEM], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = atoi(argv[startIdx + i * size + j]);
}

void imprimirMatriz(int matrix[][SIZEM], int matrixSize) {
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void imprimirMatrizFloat(float matrix[][SIZEM], int matrixSize) {
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++)
            printf("%f ", matrix[i][j]);
        printf("\n");
    }
}

void escribirMatrizArchivo(FILE *file, char titulo[], int matrix[][SIZEM], int matrixSize) {
    fprintf(file, "\n%s\n", titulo);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++)
            fprintf(file, "%d ", matrix[i][j]);
        fprintf(file, "\n");
    }
}

void escribirMatrizArchivoFloat(FILE *file, char titulo[], float matrix[][SIZEM], int matrixSize) {
    fprintf(file, "\n%s\n", titulo);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++)
            fprintf(file, "%f ", matrix[i][j]);
        fprintf(file, "\n");
    }
}


void matrixSum(int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize) {
    for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; j++)
            result[i][j] = matrixA[i][j] + matrixB[i][j];
}

void matrixSubs(int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize) {
    for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; j++)
            result[i][j] = matrixA[i][j] - matrixB[i][j];
}

void matrixMult(int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize) {
    for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; j++) {
            result[i][j] = 0;
            for (int k = 0; k < matrixSize; k++)
                result[i][j] += matrixA[i][k] * matrixB[k][j];
        }
}

void obtenerTraspuesta(int matrixA[][SIZEM], int result[][SIZEM], int matrixSize) {
    for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; j++)
            result[i][j] = matrixA[j][i];
}

void cofactor(int matrix[][SIZEM], int temp[][SIZEM],
              int filaExcluir, int colExcluir, int n) {
    int iTemp = 0, jTemp = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != filaExcluir && j != colExcluir) {
                temp[iTemp][jTemp] = matrix[i][j];
                jTemp++;
                if (jTemp == n - 1) {
                    jTemp = 0;
                    iTemp++;
                }
            }
        }
    }
}

float determinante(int matrix[][SIZEM], int matrixSize) {
    if (matrixSize == 1) return matrix[0][0];
    if (matrixSize == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    double det = 0;
    int temp[SIZEM][SIZEM];
    int signo = 1;

    for (int j = 0; j < matrixSize; j++) {
        cofactor(matrix, temp, 0, j, matrixSize);
        det += signo * matrix[0][j] * determinante(temp, matrixSize - 1);
        signo *= -1;
    }
    return (float)det;
}

int obtenerInversa(int matrix[][SIZEM], float inversa[][SIZEM], int matrixSize) {
    float det = determinante(matrix, matrixSize);
    if (det == 0) return 0;

    int temp[SIZEM][SIZEM];
    int cofactores[SIZEM][SIZEM];

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            cofactor(matrix, temp, i, j, matrixSize);
            int signo = ((i + j) % 2 == 0) ? 1 : -1;
            cofactores[i][j] = signo * (int)determinante(temp, matrixSize - 1);
        }
    }

    for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; j++)
            inversa[i][j] = cofactores[j][i] / det;

    return 1;
}

#endif

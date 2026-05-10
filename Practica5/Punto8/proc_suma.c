#include "matrices.h"

// argv[0]          = "./proc_suma"
// argv[1..N²]      = valores de matrixA (fila por fila)
// argv[N²+1..2N²]  = valores de matrixB

int main(int argc, char *argv[]) {
    int matrixA[SIZEM][SIZEM];
    int matrixB[SIZEM][SIZEM];
    int result[SIZEM][SIZEM];

    parsearMatriz(argv, 1,              matrixA, SIZEM);
    parsearMatriz(argv, 1 + SIZEM*SIZEM, matrixB, SIZEM);

    matrixSum(matrixA, matrixB, result, SIZEM);

    FILE *f = fopen("suma.txt", "w");
    if (!f) { perror("suma.txt"); return 1; }
    escribirMatrizArchivo(f, "SUMA", result, SIZEM);
    fclose(f);

    return 0;
}

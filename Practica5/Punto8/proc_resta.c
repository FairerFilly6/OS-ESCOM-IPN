#include "matrices.h"

// argv[0]          = "./proc_resta"
// argv[1..N²]      = valores de matrixA (fila por fila)
// argv[N²+1..2N²]  = valores de matrixB

int main(int argc, char *argv[]) {
    int matrixA[SIZEM][SIZEM];
    int matrixB[SIZEM][SIZEM];
    int result[SIZEM][SIZEM];

    parsearMatriz(argv, 1,              matrixA, SIZEM);
    parsearMatriz(argv, 1 + SIZEM*SIZEM, matrixB, SIZEM);

    matrixSubs(matrixA, matrixB, result, SIZEM);

    FILE *f = fopen("resta.txt", "w");
    if (!f) { perror("resta.txt"); return 1; }
    escribirMatrizArchivo(f, "RESTA", result, SIZEM);
    fclose(f);

    return 0;
}

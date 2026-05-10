#include "matrices.h"

// argv[0]          = "./proc_multiplicacion"
// argv[1..N²]      = valores de matrixA (fila por fila)
// argv[N²+1..2N²]  = valores de matrixB

int main(int argc, char *argv[]) {
    int matrixA[SIZEM][SIZEM];
    int matrixB[SIZEM][SIZEM];
    int result[SIZEM][SIZEM];

    parsearMatriz(argv, 1,              matrixA, SIZEM);
    parsearMatriz(argv, 1 + SIZEM*SIZEM, matrixB, SIZEM);

    matrixMult(matrixA, matrixB, result, SIZEM);

    FILE *f = fopen("multiplicacion.txt", "w");
    if (!f) { perror("multiplicacion.txt"); return 1; }
    escribirMatrizArchivo(f, "MULTIPLICACION", result, SIZEM);
    fclose(f);

    return 0;
}

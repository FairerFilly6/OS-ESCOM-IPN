#include "matrices.h"

// argv[0]          = "./proc_inversa"
// argv[1..N²]      = valores de matrixA (fila por fila)
// argv[N²+1..2N²]  = valores de matrixB

int main(int argc, char *argv[]) {
    int matrixA[SIZEM][SIZEM];
    int matrixB[SIZEM][SIZEM];
    float inversa[SIZEM][SIZEM];

    parsearMatriz(argv, 1,              matrixA, SIZEM);
    parsearMatriz(argv, 1 + SIZEM*SIZEM, matrixB, SIZEM);

    FILE *f = fopen("inversa.txt", "w");
    if (!f) { perror("inversa.txt"); return 1; }

    if (obtenerInversa(matrixA, inversa, SIZEM))
        escribirMatrizArchivoFloat(f, "INVERSA DE A", inversa, SIZEM);
    else
        fprintf(f, "\nINVERSA DE A\nMatriz singular, no tiene inversa.\n");

    if (obtenerInversa(matrixB, inversa, SIZEM))
        escribirMatrizArchivoFloat(f, "INVERSA DE B", inversa, SIZEM);
    else
        fprintf(f, "\nINVERSA DE B\nMatriz singular, no tiene inversa.\n");

    fclose(f);
    return 0;
}

#include "matrices.h"

// argv[0]          = "./proc_traspuesta"
// argv[1..N²]      = valores de matrixA (fila por fila)
// argv[N²+1..2N²]  = valores de matrixB

int main(int argc, char *argv[]) {
    int matrixA[SIZEM][SIZEM];
    int matrixB[SIZEM][SIZEM];
    int result[SIZEM][SIZEM];

    parsearMatriz(argv, 1,              matrixA, SIZEM);
    parsearMatriz(argv, 1 + SIZEM*SIZEM, matrixB, SIZEM);

    FILE *f = fopen("traspuesta.txt", "w");
    if (!f) { perror("traspuesta.txt"); return 1; }

    obtenerTraspuesta(matrixA, result, SIZEM);
    escribirMatrizArchivo(f, "TRASPUESTA DE A", result, SIZEM);

    obtenerTraspuesta(matrixB, result, SIZEM);
    escribirMatrizArchivo(f, "TRASPUESTA DE B", result, SIZEM);

    fclose(f);
    return 0;
}

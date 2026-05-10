#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SIZEM 10
#define N2    (SIZEM * SIZEM)   // número de elementos por matriz

// ─────────────────────────────────────────────────────────────────
//  Convierte una matriz a strings y los almacena en buf[][12]
// ─────────────────────────────────────────────────────────────────
static void matrizAArgs(int matrix[][SIZEM], int size, char buf[][12]) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            sprintf(buf[i * size + j], "%d", matrix[i][j]);
}

// ─────────────────────────────────────────────────────────────────
//  Hace fork + execv pasando las dos matrices como argv
//  argv final:  [ejecutable] [N² valores A] [N² valores B] [NULL]
// ─────────────────────────────────────────────────────────────────
static void lanzarProceso(const char *ejecutable,
                           int matrixA[][SIZEM],
                           int matrixB[][SIZEM])
{
    char bufA[N2][12];
    char bufB[N2][12];
    matrizAArgs(matrixA, SIZEM, bufA);
    matrizAArgs(matrixB, SIZEM, bufB);

    // argv: posición 0 = nombre, 1..N² = A, N²+1..2N² = B, último = NULL
    char *argv[1 + N2 + N2 + 1];
    argv[0] = (char *)ejecutable;
    for (int i = 0; i < N2; i++) argv[1 +      i] = bufA[i];    
    for (int i = 0; i < N2; i++) argv[1 + N2 + i] = bufB[i];
    argv[1 + N2 + N2] = NULL;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // ── SUSTITUCIÓN DE CÓDIGO ──
        // La imagen de este proceso hijo se reemplaza con el ejecutable
        execv(ejecutable, argv);
        // Si execv retorna, algo salió mal
        perror("execv");
        exit(1);
    }
    // El padre sigue sin bloquear (lanzará los demás procesos en paralelo)
}

// ─────────────────────────────────────────────────────────────────
//  Versión CONCURRENTE  (fork + exec)
// ─────────────────────────────────────────────────────────────────
static void modoConcurrente(int matrixA[][SIZEM], int matrixB[][SIZEM]) {

    struct timespec t0, t1;
    clock_gettime(CLOCK_REALTIME, &t0);

    // Lanzar los 5 procesos en paralelo
    lanzarProceso("./proc_suma",           matrixA, matrixB);
    lanzarProceso("./proc_resta",          matrixA, matrixB);
    lanzarProceso("./proc_multiplicacion", matrixA, matrixB);
    lanzarProceso("./proc_traspuesta",     matrixA, matrixB);
    lanzarProceso("./proc_inversa",        matrixA, matrixB);

    // Esperar a que los 5 terminen antes de leer resultados
    for (int i = 0; i < 5; i++) wait(NULL);

    // Proceso 6: leer y consolidar archivos
    pid_t pid = fork();
    if (pid < 0) { perror("fork lector"); exit(1); }
    if (pid == 0) {
        char *argv[] = { "./proc_lector", NULL };
        execv("./proc_lector", argv);
        perror("execv lector");
        exit(1);
    }
    wait(NULL);

    clock_gettime(CLOCK_REALTIME, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec) +
                     (t1.tv_nsec - t0.tv_nsec) / 1e9;
    printf("\nTiempo concurrente: %.6f segundos\n", elapsed);
}


int main(void) {
    int matrixA[SIZEM][SIZEM] = {   {1,0,0,0,0,0,0,0,0,0}, 
                                    {0,2,0,0,0,0,0,0,0,0},
                                    {0,0,3,0,0,0,0,0,0,0}, 
                                    {0,0,0,4,0,0,0,0,0,0},
                                    {0,0,0,0,5,0,0,0,0,0}, 
                                    {0,0,0,0,0,6,0,0,0,0},
                                    {0,0,0,0,0,0,7,0,0,0}, 
                                    {0,0,0,0,0,0,0,8,0,0},
                                    {0,0,0,0,0,0,0,0,9,0}, 
                                    {0,0,0,0,0,0,0,0,0,10}
                                }; 

    int matrixB[SIZEM][SIZEM] = {   {1,0,0,0,0,0,0,0,0,0}, 
                                    {0,2,0,0,0,0,0,0,0,0},
                                    {0,0,3,0,0,0,0,0,0,0}, 
                                    {0,0,0,4,0,0,0,0,0,0},
                                    {0,0,0,0,5,0,0,0,0,0}, 
                                    {0,0,0,0,0,6,0,0,0,0},
                                    {0,0,0,0,0,0,7,0,0,0}, 
                                    {0,0,0,0,0,0,0,8,0,0},
                                    {0,0,0,0,0,0,0,0,9,0}, 
                                    {0,0,0,0,0,0,0,0,0,10}
                                }; 


    modoConcurrente(matrixA, matrixB);


    
    printf("\n Archivos generados:\n");
    printf("\n Concurrente → resultadoExec.txt\n");
    


    return 0;
}

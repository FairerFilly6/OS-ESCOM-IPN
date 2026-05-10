#include <stdio.h>
#include <stdlib.h>

// Este proceso no recibe matrices.
// Lee los 5 archivos de resultado y los consolida en resultadoExec.txt,
// mostrando el contenido también en pantalla.

int main(void) {
    const char *archivos[] = {
        "suma.txt",
        "resta.txt",
        "multiplicacion.txt",
        "traspuesta.txt",
        "inversa.txt"
    };
    int n = sizeof(archivos) / sizeof(archivos[0]);

    FILE *resultado = fopen("resultadoExec.txt", "w");
    if (!resultado) { perror("resultadoExec.txt"); return 1; }

    for (int i = 0; i < n; i++) {
        FILE *f = fopen(archivos[i], "r");
        if (!f) {
            fprintf(resultado, "\nError al abrir %s\n", archivos[i]);
            printf("\nError al abrir %s\n", archivos[i]);
            continue;
        }

        char c;
        while ((c = (char)fgetc(f)) != (char)EOF) {
            fputc(c, resultado);
            putchar(c);
        }

        fclose(f);
    }

    fclose(resultado);
    printf("\n[Lector] Resultados consolidados en resultadoExec.txt\n");
    return 0;
}

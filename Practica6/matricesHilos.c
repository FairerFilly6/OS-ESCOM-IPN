#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#define SIZEM 10

void imprimirMatriz(int matrix[][SIZEM], int matrixSize){
    
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            printf("%d ", matrix[i][j] );
        }
        printf("\n");
    }
    
}

void imprimirMatrizFloat(float matrix[][SIZEM], int matrixSize){
    
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            printf("%f ", matrix[i][j] );
        }
        printf("\n");
    }
    
}

void escribirMatrizArchivo(FILE *file,
                           char titulo[],
                           int matrix[][SIZEM],
                           int matrixSize)
{
    fprintf(file, "\n%s\n", titulo);

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            fprintf(file, "%d ", matrix[i][j]);
        }

        fprintf(file, "\n");
    }
}

void escribirMatrizArchivoFloat(FILE *file,
                           char titulo[],
                           float matrix[][SIZEM],
                           int matrixSize)
{
    fprintf(file, "\n%s\n", titulo);

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            fprintf(file, "%f ", matrix[i][j]);
        }

        fprintf(file, "\n");
    }
}

void matrixSum (int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize ){
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
        
    }
}
void matrixSubs (int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize ){
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            result[i][j] = matrixA[i][j] - matrixB[i][j];
        }
        
    }
}

void matrixMult (int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize ) {
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {   
            result[i][j] = 0;
            for(int k = 0; k < matrixSize; k++)
            {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
        
    }
}


void obtenerTraspuesta(int matrixA[][SIZEM], int result[][SIZEM], int matrixSize  ){

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            result[i][j] = matrixA[j][i];
        }
    }
    

}

void cofactor(int matrix[][SIZEM],
                  int temp[][SIZEM],
                  int filaExcluir,
                  int colExcluir,
                  int n)
{
    int iTemp = 0;
    int jTemp = 0;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(i != filaExcluir && j != colExcluir)
            {
                temp[iTemp][jTemp] = matrix[i][j];
                jTemp++;

                if(jTemp == n - 1)
                {
                    jTemp = 0;
                    iTemp++;
                }
            }
        }
    }
}

float determinante(int matrix[][SIZEM], int matrixSize){
    if(matrixSize == 1)
        return matrix[0][0];

    if(matrixSize == 2)
    {
        return matrix[0][0] * matrix[1][1]
             - matrix[0][1] * matrix[1][0];
    }

    double det = 0;
    int temp[SIZEM][SIZEM];
    int signo = 1;

    for(int j = 0; j < matrixSize; j++)
    {
        cofactor(matrix, temp, 0, j, matrixSize);

        det += signo *
               matrix[0][j] *
               determinante(temp, matrixSize - 1);

        signo *= -1;
    }

    return det;

}


int obtenerInversa(int matrix[][SIZEM],
            float inversa[][SIZEM],
            int matrixSize)
{
    float det = determinante(matrix, matrixSize);

    if(det == 0)
    {
        return 0;
    }

    int temp[SIZEM][SIZEM];
    int cofactores[SIZEM][SIZEM];

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            cofactor(matrix,
                     temp,
                     i,
                     j,
                     matrixSize);

            int signo = ((i + j) % 2 == 0) ? 1 : -1;

            cofactores[i][j] =
                signo *
                determinante(temp,
                              matrixSize - 1);
        }
    }

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            inversa[i][j] =
                cofactores[j][i] / det;
        }
    }

    return 1;
}


void iniciarProcesos(){

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
    
    
    int result[SIZEM][SIZEM]; 

    int traspuesta[SIZEM][SIZEM]; 

    float inversa[SIZEM][SIZEM]; 

    pid_t pid;

    // suma
    pid = fork();
    if (pid == 0) {
        
        matrixSum(matrixA, matrixB, result, SIZEM);
        FILE *f = fopen("suma.txt", "w");
        escribirMatrizArchivo(f, "SUMA", result, SIZEM);
        fclose(f);
        exit(0);  
    }

    // resta
    pid = fork();
    if (pid == 0) {
        
        matrixSubs(matrixA, matrixB, result, SIZEM);
        FILE *f = fopen("resta.txt", "w");
        escribirMatrizArchivo(f, "Resta", result, SIZEM);
        fclose(f);
        exit(0);  
    }

    // multiplicacion
    pid = fork();
    if (pid == 0) {
        
        matrixMult(matrixA, matrixB, result, SIZEM);
        FILE *f = fopen("multiplicacion.txt", "w");
        escribirMatrizArchivo(f, "Multiplicacion", result, SIZEM);
        fclose(f);
        exit(0); 
    }

    // traspuestas
    pid = fork();
    if (pid == 0) {
        
        FILE *f = fopen("traspuesta.txt", "w");
        obtenerTraspuesta(matrixA,result,SIZEM);
        escribirMatrizArchivo(f, "Traspuesta de A", result, SIZEM);
        obtenerTraspuesta(matrixB,result,SIZEM);
        escribirMatrizArchivo(f, "Traspuesta de B", result, SIZEM);
        fclose(f);
        exit(0); 
    }

    // inversas
    pid = fork();
    if (pid == 0) {
        
        FILE *f = fopen("inversa.txt", "w");
        obtenerInversa(matrixA,inversa,SIZEM);
        escribirMatrizArchivoFloat(f, "Inversa de A", inversa, SIZEM);
        obtenerInversa(matrixB,inversa,SIZEM);
        escribirMatrizArchivoFloat(f, "Inversa de B", inversa, SIZEM);
        fclose(f);
        exit(0); 
    }

    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    // --- Proceso 6: Leer y mostrar archivos ---
    pid = fork();
    if (pid == 0) {

        // Archivos fuente
        const char *archivos[] = {
            "suma.txt",
            "resta.txt",
            "multiplicacion.txt",
            "traspuesta.txt",
            "inversa.txt"
        };

        // Archivo de resultados
        FILE *resultado = fopen("resultadoConc.txt", "w");
        if (resultado == NULL) {
            perror("Error al crear resultadoConc.txt");
            exit(1);
        }

        // Copiar cada archivo al resultado
        for (int i = 0; i < 5; i++) {

            FILE *f = fopen(archivos[i], "r");
            if (f == NULL) {
                fprintf(resultado, "\nError al abrir %s\n", archivos[i]);
                continue;
            }

            // Leer caracter por caracter y escribir en resultado
            char c;
            while ((c = fgetc(f)) != EOF) {
                fputc(c, resultado);   // escribir en archivo
                putchar(c);            // mostrar en pantalla también
            }

            fclose(f);
        }

        fclose(resultado);
        exit(0);
    }
    wait(NULL);

    return ;
}

int main(int argc, char const *argv[])
{   
    struct timespec inicio, fin;

    clock_gettime(CLOCK_REALTIME, &inicio);

    iniciarProcesos();

    clock_gettime(CLOCK_REALTIME, &fin);

    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("\nTiempo de ejecucion: %.9f segundos\n", tiempo);
    return 0;
}

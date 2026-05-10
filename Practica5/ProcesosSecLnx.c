#include <stdio.h>
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


    FILE *archivo = fopen("resultado.txt", "w");

    if(archivo == NULL)
    {
        printf("Error al abrir archivo\n");
        return ;
    }

    printf("\nMatriz A\n");
    imprimirMatriz(matrixA, SIZEM);
    escribirMatrizArchivo(archivo,"\nMatriz A\n",matrixA, SIZEM);
    printf("\nMatriz B\n");
    imprimirMatriz(matrixB, SIZEM);
    escribirMatrizArchivo(archivo,"\nMatriz B\n",matrixB, SIZEM);

    printf("\nSuma de matrices\n");
    matrixSum(matrixA, matrixB, result, SIZEM);

    imprimirMatriz(result, SIZEM);
    escribirMatrizArchivo(archivo,"\nSuma de matrices\n",result, SIZEM);

    printf("\nResta de matrices\n");
    matrixSubs(matrixA, matrixB, result, SIZEM);

    imprimirMatriz(result, SIZEM);
    escribirMatrizArchivo(archivo,"\nResta de matrices\n",result, SIZEM);

    printf("\nMultiplicacion de matrices\n");
    matrixMult(matrixA,matrixB,result,SIZEM);

    imprimirMatriz(result, SIZEM);
    escribirMatrizArchivo(archivo,"\nMultiplicacion de matrices\n",result, SIZEM);

    printf("\nTraspuesta de matriz A\n");
    obtenerTraspuesta(matrixA, traspuesta, SIZEM);

    imprimirMatriz(traspuesta, SIZEM);
    escribirMatrizArchivo(archivo,"\nTraspuesta de matriz A\n",traspuesta, SIZEM);

    printf("\nTraspuesta de matriz B\n");
    obtenerTraspuesta(matrixB, traspuesta, SIZEM);

    imprimirMatriz(traspuesta, SIZEM);
    escribirMatrizArchivo(archivo,"\nTraspuesta de matriz A\n",traspuesta, SIZEM);

    
    float det = determinante(matrixA, SIZEM);

    printf("\nDeterminante A: %f \n", det);

    printf("\nInversa de matriz A\n");
    obtenerInversa(matrixA, inversa, SIZEM );
    imprimirMatrizFloat(inversa,SIZEM );
    escribirMatrizArchivoFloat(archivo,"\nInversa de matriz A\n",inversa, SIZEM);

    printf("\nInversa de matriz B\n");
    obtenerInversa(matrixB, inversa, SIZEM );
    imprimirMatrizFloat(inversa,SIZEM );
    escribirMatrizArchivoFloat(archivo,"\nInversa de matriz B\n",inversa, SIZEM);
    
    fclose(archivo);

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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

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



void ejecutar(){
    pid_t ph, pn ;
    int pipeAH[2]; // App -> Hijo
    int pipeHA[2]; // Hijo -> App
    int pipeHN[2]; // Hijo -> Nieto
    int pipeNA[2]; // Nieto -> App

    if(pipe(pipeAH) == -1){
        perror("pipeAH");
        exit(0);
    }

    if(pipe(pipeHA) == -1){
        perror("pipeHA");
        exit(0);
    }

    if(pipe(pipeHN) == -1){
        perror("pipeHN");
        exit(0);
    }

    if(pipe(pipeNA) == -1){
        perror("pipeNA");
        exit(0);
    }


    ph = fork();
    

    //proceso hijo
    if(ph == 0){
       

        pn = fork();
        
        if (pn == 0)        //proceso nieto
        {
           printf("Proceso %d Padre %d \n", getpid(), getppid());
           printf("deberia ser el nieto\n");

            int sumaRec[SIZEM][SIZEM];
            int matrixBRec[SIZEM][SIZEM];
            int result[SIZEM][SIZEM]; 
            float inversaSum[SIZEM][SIZEM]; 

            //lectura de suma de hijo
            read(pipeHN[0], sumaRec, sizeof(sumaRec));

            //obtener inversa
            obtenerInversa(sumaRec, inversaSum, SIZEM);

            printf("desde nieto A+B'\n");
            imprimirMatrizFloat(inversaSum, SIZEM);

            //escribir inversaSum en app
            write(pipeNA[1], inversaSum, sizeof(inversaSum));

            

        }else{              //proceso hijo
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            printf("deberia ser el hijo\n");
            
            int matrixARec[SIZEM][SIZEM];
            int matrixBRec[SIZEM][SIZEM];
            int resultMult[SIZEM][SIZEM]; 
            int resultSuma[SIZEM][SIZEM]; 
            float inversaMult[SIZEM][SIZEM]; 
            
            //lectura de app
            read(pipeAH[0], matrixARec, sizeof(matrixARec));
            read(pipeAH[0], matrixBRec, sizeof(matrixBRec));

            printf("desde hijo A y B\n");
            imprimirMatriz(matrixARec, SIZEM);
            imprimirMatriz(matrixBRec, SIZEM);

            matrixMult(matrixARec,matrixBRec,resultMult,SIZEM);

            printf("desde hijo A X B\n");
            imprimirMatriz(resultMult, SIZEM);
        
            //escritura de inversa a APP
            obtenerInversa(resultMult, inversaMult, SIZEM);

            printf("desde hijo A X B '\n");
            imprimirMatrizFloat(inversaMult, SIZEM);

            write(pipeHA[1], inversaMult, sizeof(inversaMult));
            
            //Escritura de suma a nieto
            matrixSum(matrixARec, matrixBRec, resultSuma, SIZEM);

            printf("desde hijo A + B\n");
            imprimirMatriz(resultSuma, SIZEM);

            write(pipeHN[1], resultSuma, sizeof(resultSuma));


            waitpid(pn, NULL, 0);

        }

    }else                   //app
    {
        printf("Proceso %d Padre %d \n", getpid(), getppid());
        printf("deberia ser app\n");

        /*
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
                                        
                                    }; */

        int matrixA[SIZEM][SIZEM] = {   {1,0,0,0,0,0,0,0,0,0}, 
                                        {0,2,0,0,0,0,0,0,0,0},
                                        {0,0,1,0,0,0,0,0,0,0}, 
                                        {0,0,0,2,0,0,0,0,0,0},
                                        {0,0,0,0,1,0,0,0,0,0}, 
                                        {0,0,0,0,0,2,0,0,0,0},
                                        {0,0,0,0,0,0,1,0,0,0}, 
                                        {0,0,0,0,0,0,0,2,0,0},
                                        {0,0,0,0,0,0,0,0,1,0}, 
                                        {0,0,0,0,0,0,0,0,0,2}
                                    }; 

        int matrixB[SIZEM][SIZEM] = {   {1,0,0,0,0,0,0,0,0,0}, 
                                        {0,2,0,0,0,0,0,0,0,0},
                                        {0,0,1,0,0,0,0,0,0,0}, 
                                        {0,0,0,2,0,0,0,0,0,0},
                                        {0,0,0,0,1,0,0,0,0,0}, 
                                        {0,0,0,0,0,2,0,0,0,0},
                                        {0,0,0,0,0,0,1,0,0,0}, 
                                        {0,0,0,0,0,0,0,2,0,0},
                                        {0,0,0,0,0,0,0,0,1,0}, 
                                        {0,0,0,0,0,0,0,0,0,2}
                                    };                             
        float inversaMultRec[SIZEM][SIZEM];
        float inversaSumaREc[SIZEM][SIZEM]; 
        
        //escritura de A y B a HIjo

        write(pipeAH[1], matrixA, sizeof(matrixA));

        write(pipeAH[1], matrixB, sizeof(matrixB));

        //lectura de inversa de mult

        read(pipeHA[0], inversaMultRec, sizeof(inversaMultRec));

        printf("Impresion desde app inversa multiplicacion\n");
        imprimirMatrizFloat(inversaMultRec, SIZEM);

        //lectura de inversa de suma

        read(pipeNA[0], inversaSumaREc, sizeof(inversaSumaREc));

        printf("Impresion desde app inversa suma\n");
        imprimirMatrizFloat(inversaSumaREc, SIZEM);
        
        FILE *archivo = fopen("resultadoPipes.txt", "w");

        escribirMatrizArchivoFloat(archivo,"\nInversa de multiplicacion de matrices\n",inversaMultRec, SIZEM);
        printf("Escribiendo matriz inversa de multiplicacion en archivo!\n");
        escribirMatrizArchivoFloat(archivo,"\nInversa de suma de matrices\n",inversaSumaREc, SIZEM);
        printf("Escribiendo matriz inversa de suma en archivo!\n");

        waitpid(ph, NULL, 0);

        
    }
    
}

int main(int argc, char const *argv[])
{
    ejecutar();
    return 0;
}

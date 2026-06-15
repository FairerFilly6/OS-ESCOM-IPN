#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <string.h>

#define SIZEM 10

typedef struct {
    int matrixA[SIZEM][SIZEM];
    int matrixB[SIZEM][SIZEM];

} MatricesOriginales;


typedef struct {

    int suma[SIZEM][SIZEM];
    int multiplicacion[SIZEM][SIZEM];


} MatricesResultantes;


typedef struct {

    float inversaMult[SIZEM][SIZEM];
    float inversaSuma[SIZEM][SIZEM];

} MatricesInversas;


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

    

    int shmid, shmidOrig, shmidRes, shmidInv;
    int *numero;
    MatricesOriginales * mtrxsOrig;
    MatricesResultantes * mtrxsRes;
    MatricesInversas * mtrxsInv;
    
    //Generar el puntero a la shm de matrices
    shmidOrig = shmget(IPC_PRIVATE, sizeof(MatricesOriginales), IPC_CREAT | 0666);
    mtrxsOrig = (MatricesOriginales *) shmat(shmidOrig, NULL, 0);
    if(shmidOrig == -1)
    {
        perror("shmidOrig");
        exit(1);
    }
    shmidRes = shmget(IPC_PRIVATE, sizeof(MatricesResultantes), IPC_CREAT | 0666);
    mtrxsRes = (MatricesResultantes *) shmat(shmidRes, NULL, 0);
    if(shmidRes == -1)
    {
        perror("shmidRes");
        exit(1);
    }
    shmidInv = shmget(IPC_PRIVATE, sizeof(MatricesInversas), IPC_CREAT | 0666);
    mtrxsInv = (MatricesInversas *) shmat(shmidInv, NULL, 0);
    if(shmidInv == -1)
    {
        perror("shmidInv");
        exit(1);
    }


    //copiar a la shm directamente 
    memcpy(mtrxsOrig->matrixA, matrixA, sizeof(matrixA) );                 
    memcpy(mtrxsOrig->matrixB, matrixB, sizeof(matrixB));       

    //debug de la memoria compartida desde app
    printf("Lectura de matrices desde memoria compartida desde app\n");
    imprimirMatriz(mtrxsOrig->matrixA, SIZEM);
    imprimirMatriz(mtrxsOrig->matrixB, SIZEM);



    ph = fork();
    
   
    //proceso hijo
    if(ph == 0){
        pn = fork();
        if (pn == 0)        //proceso nieto
        {
           printf("Proceso %d Padre %d \n", getpid(), getppid());
           printf("deberia ser el nieto\n");

            //obtener la inversa de A+B y escribir en inversaSuma
            obtenerInversa(mtrxsCmps->suma, mtrxsCmps->inversaSuma, SIZEM);

            printf("Lectura de matrices desde memoria compartida desde nieto\n");
            imprimirMatriz(mtrxsCmps->matrixA, SIZEM);
            imprimirMatriz(mtrxsCmps->matrixB, SIZEM);

            printf("Inversa de A + B\n");
            imprimirMatrizFloat(mtrxsCmps->inversaSuma, SIZEM);

            
            

        }else{              //proceso hijo
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            printf("deberia ser el hijo\n");

            //obtenemos la suma y escribimos en suma
            matrixSum(mtrxsCmps->matrixA, mtrxsCmps->matrixB,mtrxsCmps->suma,SIZEM);

            //obtener la inversa de A X B y escribir en inversaMult
            obtenerInversa(mtrxsCmps->multiplicacion, mtrxsCmps->inversaMult, SIZEM);
           
           
            printf("Lectura de matrices desde memoria compartida desde hijo\n");
            imprimirMatriz(mtrxsCmps->matrixA, SIZEM);
            imprimirMatriz(mtrxsCmps->matrixB, SIZEM);
            printf("Suma de A + B\n");
            imprimirMatriz(mtrxsCmps->suma, SIZEM);

            printf("Inversa de A X B\n");
            imprimirMatrizFloat(mtrxsCmps->inversaMult, SIZEM);

            
            

            waitpid(pn, NULL, 0);

        }

    }else                   //app
    {
        printf("Proceso %d Padre %d \n", getpid(), getppid());
        printf("deberia ser app\n");

        //obtenemos la multiplicacion y escribimos en multiplicacion
        matrixMult(mtrxsCmps->matrixA, mtrxsCmps->matrixB,mtrxsCmps->multiplicacion,SIZEM);

        printf("Multiplicacion de A X B\n");
        imprimirMatriz(mtrxsCmps->multiplicacion, SIZEM);

       
        
        waitpid(ph, NULL, 0);

        
    }

    printf("Resultado final de todo\n");
    
    printf("Suma de A + B\n");
    imprimirMatriz(mtrxsCmps->suma, SIZEM);

    printf("Inversa de A + B\n");
    imprimirMatrizFloat(mtrxsCmps->inversaSuma, SIZEM);

    printf("Multiplicacion de A X B\n");
    imprimirMatriz(mtrxsCmps->multiplicacion, SIZEM);

    printf("Inversa de A X B\n");
    imprimirMatrizFloat(mtrxsCmps->inversaMult, SIZEM);

    FILE *archivo = fopen("resultadoSHM.txt", "w");

    escribirMatrizArchivoFloat(archivo,"\nInversa de multiplicacion de matrices\n",mtrxsCmps->inversaMult, SIZEM);
    printf("Escribiendo matriz inversa de multiplicacion en archivo!\n");
    escribirMatrizArchivoFloat(archivo,"\nInversa de suma de matrices\n",mtrxsCmps->inversaSuma, SIZEM);
    printf("Escribiendo matriz inversa de suma en archivo!\n");

    fclose(archivo);

    //close de la shm
    shmdt(mtrxsCmps);
    shmctl(shmidMtrx, IPC_RMID, NULL);
}

int main(int argc, char const *argv[])
{
    ejecutar();
    return 0;
}

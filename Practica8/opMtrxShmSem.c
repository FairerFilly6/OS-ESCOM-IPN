#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>

#define SIZEM 10


typedef struct 
{
    int suma[SIZEM][SIZEM];
    int resta[SIZEM][SIZEM];
    int multiplicacion[SIZEM][SIZEM];
} MtrxsBin;

typedef struct 
{
    int traspuestaA[SIZEM][SIZEM];
    int traspuestaB[SIZEM][SIZEM];
    float inversaA[SIZEM][SIZEM];
    float inversaB[SIZEM][SIZEM];
} MtrxsUn;






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

void escribirMatrizArchivo(FILE *file, char titulo[], int matrix[][SIZEM], int matrixSize)
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

void escribirMatrizArchivoFloat(FILE *file, char titulo[], float matrix[][SIZEM], int matrixSize)
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

void cofactor(int matrix[][SIZEM], int temp[][SIZEM], int filaExcluir, int colExcluir, int n)
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


int obtenerInversa(int matrix[][SIZEM], float inversa[][SIZEM], int matrixSize)
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

void waitSem(int semid, int num)
{
    struct sembuf op;

    op.sem_num = num;
    op.sem_op = -1;
    op.sem_flg = 0;

    semop(semid, &op, 1);
}

void signalSem(int semid, int num)
{
    struct sembuf op;

    op.sem_num = num;
    op.sem_op = 1;
    op.sem_flg = 0;

    semop(semid, &op, 1);
}




void ejecutar(){
    pid_t ps, pr, pm, pt, pi, recolector;

    //creacion de 5 semaforos para cada proceso
    int semid;

    semid = semget(IPC_PRIVATE, 5, IPC_CREAT | 0666 );

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
                                
    int shmBin, shmUn;
    MtrxsBin * mtrxsBin;
    MtrxsUn * mtrxsUn;
    
    //Generar el puntero a la shm de matrices

    shmBin = shmget(IPC_PRIVATE, sizeof(MtrxsBin), IPC_CREAT | 0666);
    mtrxsBin = (MtrxsBin *) shmat(shmBin, NULL, 0);

    if(shmBin == -1)
    {
        perror("shmget");
        exit(1);
    }

    shmUn = shmget(IPC_PRIVATE, sizeof(MtrxsUn), IPC_CREAT | 0666);
    mtrxsUn = (MtrxsUn *) shmat(shmUn, NULL, 0);

    if(shmUn == -1)
    {
        perror("shmget");
        exit(1);
    }

    //inicializacion de los semaforos en 0 
    semctl(semid, 0, SETVAL, 0); // suma
    semctl(semid, 1, SETVAL, 0); // resta
    semctl(semid, 2, SETVAL, 0); // mult
    semctl(semid, 3, SETVAL, 0); // traspuesta
    semctl(semid, 4, SETVAL, 0); // inversa

                                
    recolector = fork();

    if(recolector == 0)
    {
        ps = fork();

        if(ps == 0)
        {


            printf("SUMA\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            matrixSum(matrixA, matrixB, mtrxsBin->suma, SIZEM);
            signalSem(semid, 0);
            printf("Suma de matrices\n");
            imprimirMatriz(mtrxsBin->suma, SIZEM);
            
            exit(0);
        }

        pr = fork();

        if(pr == 0)
        {
            printf("RESTA\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            matrixSubs(matrixA, matrixB, mtrxsBin->resta, SIZEM);
            signalSem(semid, 1);
            printf("Resta de matrices\n");
            imprimirMatriz(mtrxsBin->resta, SIZEM);
            exit(0);
        }

        pm = fork();

        if(pm == 0)
        {
            printf("MULT\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            matrixMult(matrixA, matrixB, mtrxsBin->multiplicacion, SIZEM);
            signalSem(semid, 2);
            printf("Multiplicacion de matrices\n");
            imprimirMatriz(mtrxsBin->multiplicacion, SIZEM);
            exit(0);
        }

        pt = fork();

        if(pt == 0)
        {
            printf("Traspuesta\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            obtenerTraspuesta(matrixA, mtrxsUn->traspuestaA, SIZEM);
            obtenerTraspuesta(matrixB, mtrxsUn->traspuestaB, SIZEM);
            signalSem(semid, 3);
            printf("Traspuesta de matriz A\n");
            imprimirMatriz(mtrxsUn->traspuestaA, SIZEM);
            printf("Traspuesta de matriz B\n");
            imprimirMatriz(mtrxsUn->traspuestaB, SIZEM);
            exit(0);
        }
        
        pi = fork();

        if(pi == 0)
        {
            printf("Inversa\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            obtenerInversa(matrixA, mtrxsUn->inversaA, SIZEM);
            obtenerInversa(matrixB, mtrxsUn->inversaB, SIZEM);
            signalSem(semid, 4);
            printf("Inversa de matriz A\n");
            imprimirMatrizFloat(mtrxsUn->inversaA, SIZEM);
            printf("Inversa de matriz B\n");
            imprimirMatrizFloat(mtrxsUn->inversaB, SIZEM);
            exit(0);
        }

        //esperar a que todos los semaforos indiquen que funciona
        waitSem(semid, 0);
        waitSem(semid, 1);
        waitSem(semid, 2);
        waitSem(semid, 3);
        waitSem(semid, 4);

        waitpid(ps,NULL,0);
        waitpid(pr,NULL,0);
        waitpid(pm,NULL,0);
        waitpid(pt,NULL,0);
        waitpid(pi,NULL,0);

        printf("RECOLECTOR\n");
        printf("Proceso %d Padre %d \n", getpid(), getppid());


         FILE *archivo = fopen("opMtrxShmSem.txt", "w");

        escribirMatrizArchivo(archivo,"\nSuma de matrices\n",mtrxsBin->suma, SIZEM);
        printf("Escribiendo suma de matrices en archivo!\n");

        escribirMatrizArchivo(archivo,"\nResta de matrices\n",mtrxsBin->resta, SIZEM);
        printf("Escribiendo resta de matrices en archivo!\n");

        escribirMatrizArchivo(archivo,"\nMultiplicacion de matrices\n",mtrxsBin->multiplicacion, SIZEM);
        printf("Escribiendo multiplicacion de matrices en archivo!\n");

        escribirMatrizArchivo(archivo,"\nTraspuesta de matriz A\n",mtrxsUn->traspuestaA, SIZEM);
        printf("Escribiendo traspuesta de A en archivo!\n");

        escribirMatrizArchivo(archivo,"\nTraspuesta de matriz B\n",mtrxsUn->traspuestaB, SIZEM);
        printf("Escribiendo traspuesta de B en archivo!\n");

        escribirMatrizArchivoFloat(archivo,"\nInversa de matriz A\n",mtrxsUn->inversaA, SIZEM);
        printf("Escribiendo inversa de A en archivo!\n");

        escribirMatrizArchivoFloat(archivo,"\nInversa de matriz B\n",mtrxsUn->inversaB, SIZEM);
        printf("Escribiendo inversa de B en archivo!\n");
        
        semctl(semid, 0, IPC_RMID);
        fclose(archivo);
        shmdt(mtrxsBin);
        shmctl(shmBin, IPC_RMID, NULL);
        shmdt(mtrxsUn);
        shmctl(shmUn, IPC_RMID, NULL);


        exit(0);
    }
    else
    {
        waitpid(recolector,NULL,0);

        printf("APP\n");
        printf("Proceso %d Padre %d \n", getpid(), getppid());
    }


}


int main(int argc, char const *argv[])
{
    struct timespec inicio, fin;

    clock_gettime(CLOCK_REALTIME, &inicio);

    ejecutar();

    clock_gettime(CLOCK_REALTIME, &fin);

    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("\nTiempo de ejecucion: %.9f segundos\n", tiempo);
    return 0;
}

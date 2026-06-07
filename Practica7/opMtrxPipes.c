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
    pid_t ps, pr, pm, pt, pi, recolector;


    int pipeSum[2];     // suma -> recolector
    int pipeSub[2];     // resta -> recolector
    int pipeMult[2];    // mult -> recolector
    int pipeTras[2];    // traspuesta -> recolector
    int pipeInv[2];     // inversa -> recolector
    int pipeRec[2];     // recolector -> app

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


    if(pipe(pipeSum) == -1){
        perror("pipeSum");
        exit(0);
    }

    if(pipe(pipeSub) == -1){
        perror("pipeSub");
        exit(0);
    }

    if(pipe(pipeMult) == -1){
        perror("pipeMult");
        exit(0);
    }

    if(pipe(pipeTras) == -1){
        perror("pipeTras");
        exit(0);
    }

    if(pipe(pipeInv) == -1){
        perror("pipeInv");
        exit(0);
    }
    
    if(pipe(pipeRec) == -1){
        perror("pipeRec");
        exit(0);
    }

    recolector = fork();

    if(recolector == 0)
    {
        ps = fork();

        if(ps == 0)
        {
            printf("SUMA\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            int suma[SIZEM][SIZEM]; 
            matrixSum(matrixA, matrixB, suma, SIZEM);
            printf("Suma de matrices\n");
            imprimirMatriz(suma, SIZEM);
            write(pipeSum[1], suma, sizeof(suma));


            exit(0);
        }

        pr = fork();

        if(pr == 0)
        {
            printf("RESTA\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            int resta[SIZEM][SIZEM]; 
            matrixSubs(matrixA, matrixB, resta, SIZEM);
            printf("Resta de matrices\n");
            imprimirMatriz(resta, SIZEM);
            write(pipeSub[1], resta, sizeof(resta));
            exit(0);
        }

        pm = fork();

        if(pm == 0)
        {
            printf("MULT\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            int multiplicacion[SIZEM][SIZEM]; 
            matrixMult(matrixA, matrixB, multiplicacion, SIZEM);
            printf("Multiplicacion de matrices\n");
            imprimirMatriz(multiplicacion, SIZEM);
            write(pipeMult[1], multiplicacion, sizeof(multiplicacion));
            exit(0);
        }

        pt = fork();

        if(pt == 0)
        {
            printf("Traspuesta\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            int traspuestaA[SIZEM][SIZEM]; 
            int traspuestaB[SIZEM][SIZEM]; 
            obtenerTraspuesta(matrixA, traspuestaA, SIZEM);
            obtenerTraspuesta(matrixA, traspuestaB, SIZEM);
            printf("Traspuesta de matriz A\n");
            imprimirMatriz(traspuestaA, SIZEM);
            printf("Traspuesta de matriz B\n");
            imprimirMatriz(traspuestaB, SIZEM);
            write(pipeTras[1], traspuestaA, sizeof(traspuestaA));
            write(pipeTras[1], traspuestaB, sizeof(traspuestaB));
            exit(0);
        }
        
        pi = fork();

        if(pi == 0)
        {
            printf("Inversa\n");
            printf("Proceso %d Padre %d \n", getpid(), getppid());
            float inversaA[SIZEM][SIZEM]; 
            float inversaB[SIZEM][SIZEM]; 
            obtenerInversa(matrixA, inversaA, SIZEM);
            obtenerInversa(matrixB, inversaB, SIZEM);
            printf("Inversa de matriz A\n");
            imprimirMatrizFloat(inversaA, SIZEM);
            printf("Inversa de matriz B\n");
            imprimirMatrizFloat(inversaB, SIZEM);
            write(pipeInv[1], inversaA, sizeof(inversaA));
            write(pipeInv[1], inversaB, sizeof(inversaA));
            exit(0);
        }


        waitpid(ps,NULL,0);
        waitpid(pr,NULL,0);
        waitpid(pm,NULL,0);
        waitpid(pt,NULL,0);
        waitpid(pi,NULL,0);

        printf("RECOLECTOR\n");
        printf("Proceso %d Padre %d \n", getpid(), getppid());

        int sumaRec[SIZEM][SIZEM]; 
        int restaRec[SIZEM][SIZEM]; 
        int multRec[SIZEM][SIZEM]; 
        int trasARec[SIZEM][SIZEM]; 
        int trasBRec[SIZEM][SIZEM]; 
        float invARec[SIZEM][SIZEM]; 
        float invBRec[SIZEM][SIZEM]; 

        read(pipeSum[0], sumaRec, sizeof(sumaRec));
        read(pipeSub[0], restaRec, sizeof(restaRec));
        read(pipeMult[0], multRec, sizeof(multRec));
        read(pipeTras[0], trasARec, sizeof(trasARec));
        read(pipeTras[0], trasBRec, sizeof(trasBRec));
        read(pipeInv[0], invARec, sizeof(invARec));
        read(pipeInv[0], invBRec, sizeof(invBRec));


        imprimirMatriz(sumaRec, SIZEM);
        imprimirMatriz(restaRec, SIZEM);
        imprimirMatriz(multRec, SIZEM);
        imprimirMatriz(trasARec, SIZEM);
        imprimirMatriz(trasBRec, SIZEM);
        imprimirMatrizFloat(invARec, SIZEM);
        imprimirMatrizFloat(invBRec, SIZEM);

        FILE *archivo = fopen("opMtrxPipes.txt", "w");

        escribirMatrizArchivo(archivo,"\nSuma de matrices\n",sumaRec, SIZEM);
        printf("Escribiendo suma de matrices en archivo!\n");

        escribirMatrizArchivo(archivo,"\nResta de matrices\n",restaRec, SIZEM);
        printf("Escribiendo resta de matrices en archivo!\n");

        escribirMatrizArchivo(archivo,"\nMultiplicacion de matrices\n",multRec, SIZEM);
        printf("Escribiendo multiplicacion de matrices en archivo!\n");

        escribirMatrizArchivo(archivo,"\nTraspuesta de matriz A\n",trasARec, SIZEM);
        printf("Escribiendo traspuesta de A en archivo!\n");

        escribirMatrizArchivo(archivo,"\nTraspuesta de matriz B\n",trasBRec, SIZEM);
        printf("Escribiendo traspuesta de B en archivo!\n");

        escribirMatrizArchivoFloat(archivo,"\nInversa de matriz A\n",invARec, SIZEM);
        printf("Escribiendo inversa de A en archivo!\n");

        escribirMatrizArchivoFloat(archivo,"\nInversa de matriz B\n",invBRec, SIZEM);
        printf("Escribiendo inversa de B en archivo!\n");

        fclose(archivo);

    


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
    return 0;
}

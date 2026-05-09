#include <stdio.h>

#define SIZEM 10

void imprimirMatriz(int matrix[][SIZEM], int matrixSize){
    
    for (int i = 0; i < matrixSize-1; i++)
    {
        for (int j = 0; j < matrixSize-1; j++)
        {
            printf("%d ", matrix[i][j] );
        }
        printf("\n");
    }
    
}

void matrixSum (int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize ){
    for (int i = 0; i < matrixSize-1; i++)
    {
        for (int j = 0; j < matrixSize-1; j++)
        {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
        
    }
}
void matrixSubs (int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize ){
    for (int i = 0; i < matrixSize-1; i++)
    {
        for (int j = 0; j < matrixSize-1; j++)
        {
            result[i][j] = matrixA[i][j] - matrixB[i][j];
        }
        
    }
}

void matrixMult (int matrixA[][SIZEM], int matrixB[][SIZEM], int result[][SIZEM], int matrixSize ) {
    for (int i = 0; i < matrixSize-1; i++)
    {
        for (int j = 0; j < matrixSize-1; j++)
        {
            for(int k = 0; k < matrixSize-1; k++)
            {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
        
    }
}



void iniciarProcesos(){

    int matrixA[SIZEM][SIZEM] = {  {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}
                                }; 

    int matrixB[SIZEM][SIZEM] = {  {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
                                    {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}
                                }; 

    int result[SIZEM][SIZEM]; 

    imprimirMatriz(matrixA, SIZEM);
    imprimirMatriz(matrixB, SIZEM);

    matrixSum(matrixA, matrixB, result, SIZEM);

    imprimirMatriz(result, SIZEM);

    matrixMult(matrixA,matrixB,result,SIZEM);

    imprimirMatriz(result, SIZEM);
    

    


}


int main(int argc, char const *argv[])
{
    iniciarProcesos();
    return 0;
}

#include <stdio.h>

#define SIZEM 4

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

    // int matrixA[SIZEM][SIZEM] = {  {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}
    //                             }; 

    // int matrixB[SIZEM][SIZEM] = {  {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10},
    //                                 {1,2,3,4,5,6,7,8,9,10}, {1,2,3,4,5,6,7,8,9,10}
    //                             }; 
    
    int matrixA[SIZEM][SIZEM] = {  {9,8,7,6}, {2,4,5,6},{1,4,5,8}, {2,9,8,6}  }; 

    int matrixB[SIZEM][SIZEM] = {  {9,8,7,6}, {2,4,5,6},{1,4,5,8}, {2,9,8,6}  };                             

    int result[SIZEM][SIZEM]; 

    int traspuesta[SIZEM][SIZEM]; 

    float inversa[SIZEM][SIZEM]; 

    printf("\nMatriz A\n");
    imprimirMatriz(matrixA, SIZEM);
    printf("\nMatriz B\n");
    imprimirMatriz(matrixB, SIZEM);

    printf("\nSuma de matrices\n");
    matrixSum(matrixA, matrixB, result, SIZEM);

    imprimirMatriz(result, SIZEM);

    printf("\nResta de matrices\n");
    matrixSubs(matrixA, matrixB, result, SIZEM);

    imprimirMatriz(result, SIZEM);

    printf("\nMultiplicacion de matrices\n");
    matrixMult(matrixA,matrixB,result,SIZEM);

    imprimirMatriz(result, SIZEM);

    printf("\nTraspuesta de matriz A\n");
    obtenerTraspuesta(matrixA, traspuesta, SIZEM);

    imprimirMatriz(traspuesta, SIZEM);

    
    float det = determinante(matrixA, SIZEM);

    printf("\nDeterminante A: %f \n", det);

    printf("\nInversa de matriz A\n");
    obtenerInversa(matrixA, inversa, SIZEM );
    imprimirMatrizFloat(inversa,SIZEM );
    


}


int main(int argc, char const *argv[])
{
    iniciarProcesos();
    return 0;
}

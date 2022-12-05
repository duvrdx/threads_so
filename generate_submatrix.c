#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0

#define C_MATRIX 4
#define L_MATRIX 4
#define C_SUBMATRIX 2
#define L_SUBMATRIX 2

#define SUBMATRIX_MAX (C_MATRIX*L_MATRIX)/(C_SUBMATRIX*L_SUBMATRIX)

typedef int boolean;

typedef struct {
    int es[2];
    int di[2];
    boolean state;
} coords;


coords * printSubmatrixes(int matrix[L_MATRIX][C_MATRIX]){
    int es[2] = {0, 0};
    int di[2];

    coords *arrayCoords = (coords *)malloc(sizeof(coords) * SUBMATRIX_MAX);
    coords *auxCoords;


    for(int i = 0; i < SUBMATRIX_MAX; i++){

        if(es[0] >= C_MATRIX){
            es[0] = 0;
            es[1] = es[1] + L_SUBMATRIX;
        }

        di[0] = es[0] + C_SUBMATRIX - 1;
        di[1] = es[1] + L_SUBMATRIX - 1;

        // FALTA COLOCAR DADOS NO VETOR DE STRUCT
        
        es[0] += C_SUBMATRIX;
    }
}

int main(){
    int matrix[L_MATRIX][C_MATRIX];
    printSubmatrixes(matrix);
}
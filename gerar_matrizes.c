#include <stdlib.h> #include <stdio.h>

#define true 1
#define false 0

#define C_MATRIX 5
#define L_MATRIX 4
#define C_SUBMATRIX 2
#define L_SUBMATRIX 2
#define SUBMATRIX_MAX (C_MATRIX * L_MATRIX) / (C_SUBMATRIX * L_SUBMATRIX)

typedef int boolean;

int *generateRandomMatrix(int seed) {
  srand(seed);
  printf("\n");
  int *matrix = (int *)malloc(sizeof(int) * L_MATRIX * C_MATRIX);

  for (int i = 0; i < C_MATRIX; i++) {
    for (int j = 0; j < L_MATRIX; j++) {
      *(matrix + i * C_MATRIX + j) = rand() % 32000;
    }
  }

  return matrix;
}

void printMatrix(int *matrix) {
  for (int i = 0; i < C_MATRIX; i++) {
    for (int j = 0; j < L_MATRIX; j++) {
      printf("%d ", *(matrix + i * C_MATRIX + j));
    }
    printf("\n");
  }
}

boolean isPrime(int number) {
  for (int count = 2; count <= number / 2; count++)
    if ((number % count) == 0)
      return false;

  return true;
}

// TODO
int *getSubmatrixIndexes(int submatrix) {}

int sequencialSearch(int *matrix) {
  int count = 0;
  for (int i = 0; i < L_MATRIX; i++) {
    for (int j = 0; j < C_MATRIX; j++) {
      if (isPrime(*(matrix + i * C_MATRIX + j)))
        count++;
    }
  }

  return count;
}

int main() {
  int *matrix = generateRandomMatrix(154545654);
  printMatrix(matrix);
  printf("Dimensoes da Matriz: %d x %d\n", L_MATRIX, C_MATRIX);
  printf("Tem %d primos na matriz ", sequencialSearch(matrix));
  free(matrix);
}

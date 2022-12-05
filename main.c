#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define true 1
#define false 0

#define NUM_THREADS 1
#define RAND_RANGE 32000
#define RAND_SEED 1234

#define C_MATRIX 10000
#define R_MATRIX 10000
#define C_SUBMATRIX 10000
#define R_SUBMATRIX 10000
#define SUBMATRIX_MAX (C_MATRIX*R_MATRIX)/(C_SUBMATRIX*R_SUBMATRIX)

typedef int boolean;

typedef struct {
    int upperLeft[2];
    int lowerRight[2];
    boolean state;
} submatrix;

int** MATRIX;
int PRIME_COUNT = 0;

pthread_mutex_t COUNT_MUTEX;
pthread_mutex_t STATE_MUTEX;


submatrix* getSubmatrixes() {
    int upperLeft[2] = { 0, 0 };
    int lowerRight[2];

    submatrix* arraySubmatrix = (submatrix*)malloc(sizeof(submatrix)
        * SUBMATRIX_MAX);

    if (!arraySubmatrix) {
        perror("Could not allocate memory for array of submatrices, exiting.\n");
        exit(-3);
    }

    for (int i = 0; i < SUBMATRIX_MAX; i++) {

        if (upperLeft[0] >= C_MATRIX) {
            upperLeft[0] = 0;
            upperLeft[1] = upperLeft[1] + R_SUBMATRIX;
        }

        lowerRight[0] = upperLeft[0] + C_SUBMATRIX - 1;
        lowerRight[1] = upperLeft[1] + R_SUBMATRIX - 1;

        arraySubmatrix[i].upperLeft[0] = upperLeft[0];
        arraySubmatrix[i].upperLeft[1] = upperLeft[1];
        arraySubmatrix[i].lowerRight[0] = lowerRight[0];
        arraySubmatrix[i].lowerRight[1] = lowerRight[1];
        arraySubmatrix[i].state = false;

        upperLeft[0] += C_SUBMATRIX;
    }
    return(arraySubmatrix);
}

int** generateRandomMatrix(int seed) {
    srand(seed);
    int** matrix = malloc(sizeof(int*) * R_MATRIX);
    if (!matrix) {
        perror("Could not allocate memory for rows of main matrix, exiting.\n");
        exit(-1);
    }

    for (int i = 0; i < R_MATRIX; i++) {
        matrix[i] = malloc(C_MATRIX * sizeof(int));
        if (!matrix[i]) {
            perror("Could not allocate memory for columns of main matrix, exiting.\n");
            exit(-2);
        }
        for (int j = 0; j < C_MATRIX; j++)
            matrix[i][j] = rand() % RAND_RANGE;
    }

    return matrix;
}

void printMatrix(int** matrix) {
    for (int i = 0; i < C_MATRIX; i++) {
        for (int j = 0; j < R_MATRIX; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

boolean isPrime(int number) {
    if (number == 0 || number == 1) return false;
    for (int count = 2; count <= sqrt(number); count++)
        if ((number % count) == 0) return false;

    return true;
}

int sequencialSearch(int** matrix) {
    int count = 0;
    for (int i = 0; i < R_MATRIX; i++) {
        for (int j = 0; j < C_MATRIX; j++)
            if (isPrime(matrix[i][j])) count++;
    }

    return count;
}

void* countPrimes(void* param) {
    int count;
    boolean canRun = false;
    submatrix* currentSubmatrix = (submatrix*)param;

    for (int sbmxCount = 0; sbmxCount < SUBMATRIX_MAX; sbmxCount++) {
        count = 0;

        pthread_mutex_lock(&STATE_MUTEX);
        if (!(currentSubmatrix[sbmxCount].state)) {
            currentSubmatrix[sbmxCount].state = true;
            canRun = true;
        }
        pthread_mutex_unlock(&STATE_MUTEX);

        if (canRun)
            for (int i = currentSubmatrix[sbmxCount].upperLeft[1];
                i <= currentSubmatrix[sbmxCount].lowerRight[1]; i++)
                for (int j = currentSubmatrix[sbmxCount].upperLeft[0];
                    j <= currentSubmatrix[sbmxCount].lowerRight[0]; j++)
                    if (isPrime(MATRIX[i][j])) count++;

        if (count > 0) {
            pthread_mutex_lock(&COUNT_MUTEX);
            PRIME_COUNT += count;
            pthread_mutex_unlock(&COUNT_MUTEX);
        }
        canRun = false;
    }

    return NULL;
}

int main() {
    double timeSpent_s = 0.0;
    double timeSpent_p = 0.0;
    clock_t begin_s = 0;
    clock_t end_s = 0;
    clock_t begin_p = 0;
    clock_t end_p = 0;

    int sequentialPrimes;

    submatrix* sbmxArray = getSubmatrixes();

    pthread_t workers[NUM_THREADS];
    pthread_attr_t wk_attr[NUM_THREADS];

    long long int matrixMemorySize = 4 * R_MATRIX * C_MATRIX;

    printf("Random seed: %d\n", RAND_SEED);
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Number of rows of Matrix: %d\n", R_MATRIX);
    printf("Number of columns of Matrix: %d\n", C_MATRIX);
    printf("Number of rows of submatrix: %d\n", R_SUBMATRIX);
    printf("Number of columns of submatrix: %d\n", R_SUBMATRIX);
    printf("Number of submatrices: %d\n", SUBMATRIX_MAX);
    printf("Size in memory of Matrix (bytes): %lld\n", matrixMemorySize);
    printf("Size in memory of Matrix (KiB): %d\n", (int)matrixMemorySize / 1024);
    printf("Size in memory of Matrix (MiB): %d\n", (int)matrixMemorySize
        / 1048576);
    printf("Size in memory of Matrix (GiB): %d\n\n\n", (int)matrixMemorySize
        / 1073741824);


    printf("Matrix generation started ... ");
    MATRIX = generateRandomMatrix(RAND_SEED);
    printf("Matrix generation ended.\n\n\n");

    pthread_mutex_init(&COUNT_MUTEX, NULL);
    pthread_mutex_init(&STATE_MUTEX, NULL);

    printf("Sequential search\n");
    begin_s = clock();
    sequentialPrimes = sequencialSearch(MATRIX);
    end_s = clock();
    printf("Total number of primes:\t%d\n", sequentialPrimes);
    timeSpent_s += (double)(end_s - begin_s) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", timeSpent_s);


    printf("\n\nParallel search\nThreads:\t%d \n\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_attr_init(&wk_attr[i]);
        pthread_create(&workers[i], NULL, countPrimes, sbmxArray);
    }

    begin_p = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }
    end_p = clock();

    printf("Total number of primes:\t%d\n", PRIME_COUNT);
    timeSpent_p += (double)(end_p - begin_p) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", timeSpent_p/NUM_THREADS);

    free(MATRIX);
    free(sbmxArray);

    return(0);
}
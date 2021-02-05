// matric operations

// initialise matrices, referencing available in 
// https://stackoverflow.com/questions/3911400/how-to-pass-2d-array-matrix-in-a-function-in-c
// #include <iostream>
#include <iostream>
#include <stdlib.h>


int malloc2dint(int ***array, int n, int m) {

    /* allocate the n*m contiguous items */
    int *p = (int *)malloc(n*m*sizeof(int));
    if (!p) return -1;

    /* allocate the row pointers into the memory */
    (*array) = (int **)malloc(n*sizeof(int*));
    if (!(*array)) {
       free(p);
       return -1;
    }

    /* set up the pointers into the contiguous memory */
    for (int i=0; i<n; i++) 
       (*array)[i] = &(p[i*m]);

    return 0;
}

int free2dint(int ***array) {
    /* free the memory - the first element of the array is at the start */
    free(&((*array)[0][0]));

    /* free the pointers into the memory */
    free(*array);

    return 0;
}

int print_matrix(int** mat, int rows, int cols){

    int i,j;
    for (int i = 0; i < rows; ++i)
    {   
        std::cout << "|\t";
        for (int j = 0; j < cols; ++j)
        {
            std::cout << mat[i][j] << "\t";
            // std::cout <<i <<j;
        }
        std::cout << "|" << std::endl;
    }

    return 0;
}


int** matrix_multiplication(int **A, int **B, int M, int K, int N)
{
    int i, j, k;
    int **C;
    malloc2dint(&C, M, N);

    for (i=0; i<M; i++)
    {
        for (j=0; j<N; j++)
        {   
            C[i][j] = 0;
            for (int k = 0; k < K; k++)
            {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    return C;
}


// taken from https://stackoverflow.com/questions/5104847/mpi-bcast-a-dynamic-2d-array
// this also explains how to Bcast arrays
// needs to have a CONTIGUOUS array
int malloc2dfloat(float ***array, int n, int m) {

    /* allocate the n*m contiguous items */
    float *p = (float *)malloc(n*m*sizeof(float));
    if (!p) return -1;

    /* allocate the row pointers into the memory */
    (*array) = (float **)malloc(n*sizeof(float*));
    if (!(*array)) {
       free(p);
       return -1;
    }

    /* set up the pointers into the contiguous memory */
    for (int i=0; i<n; i++) 
       (*array)[i] = &(p[i*m]);

    return 0;
}

int free2dfloat(float ***array) {
    /* free the memory - the first element of the array is at the start */
    free(&((*array)[0][0]));

    /* free the pointers into the memory */
    free(*array);

    return 0;
}

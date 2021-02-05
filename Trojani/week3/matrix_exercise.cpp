
#include <iostream>
#include <stdlib.h>
#include <mpi.h>
#include <typeinfo>
#include <array>
#include "matrix_operation.h"
using namespace std;
#include<unistd.h>
#include <string>
#include <sstream>
// learn the vector library at some point!!
#include <vector>
unsigned int microsecond = 1000000;


int main(int argc, char const *argv[])
{

    int rank, nproc, ierr;
    MPI_Comm comm;

    comm  = MPI_COMM_WORLD;
    stringstream str;
    str << argv[1]; int N; str >> N;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &rank);            
    MPI_Comm_size(comm, &nproc);

    if (N % nproc !=0)
    {
        cout << "Error size of problem not divisible by number of processors"<<endl;
        MPI_Finalize();
    }
    else if (nproc > N)
    {
        cout << "Error: I haven't done this case" <<endl;
        MPI_Finalize();
    }

    int mat_size = N*N;
    int root = 0;
    int **B, **A, **C;
    int **D;
    malloc2dint(&A, N,N);
    malloc2dint(&B, N,N);
    malloc2dint(&C, N,N);
    malloc2dint(&D, N,N);

    int quot = N / nproc;
    int **A_quot_rows, **C_quot_rows;
    malloc2dint(&A_quot_rows, quot,N);
    malloc2dint(&C_quot_rows, quot,N);

    if (rank == root)
    {
        cout << "Number of processors is "<< nproc<< endl;
        cout << "Number of rows is " << N <<endl;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {   
                // we calculate A for purely testing reasons
                A[i][j] = (N - j + i +1)*(i+1);
                // --------------------------
                B[i][j] = (j+i+2)*(N-j);
            }
        }
        C = matrix_multiplication(A,B, N,N,N);
        print_matrix(C, N,N);
    }
    MPI_Bcast(&(B[0][0]) , mat_size, MPI_INT, root, comm);
    // NOTE: Bcast sends local copy to processors. as can be seen by printing &B
    // root process computes B and sends it to all to use
    // Even when there are more rows than processors, we send the matric B to all processors

    for (int i = 0; i < quot; ++i)
    {   
        for (int j = 0; j < N; ++j)
        {   
            A_quot_rows[i][j] = (N - j + i + quot*rank +1)*(i + quot*rank + 1);
        }
    }

    C_quot_rows = matrix_multiplication(A_quot_rows, B, quot, N, N);

  // MPI_GATHER(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm)
  //   IN       sendbuf                 starting address of send buffer
  //   IN       sendcount               number of elements in send buffer
  //   IN       sendtype                data type of send buffer elements
  //   OUT      recvbuf                 address of receive buffer
  //   IN       recvcount               number of elements for any single
  //                                    receive
  //   IN       recvtype                data type of recv buffer elements
  //   IN       root                    rank of reciving process
  //   IN       comm                    communicator

    // Gather result into matrix D and compare with normal computation in matric C
    MPI_Gather(&(C_quot_rows[0][0]), N*quot, MPI_INT, &(D[0][0]), N*quot, MPI_INT, root, comm);
    
    if (rank ==0)
    {
        // checks results
        cout <<endl;
        print_matrix(D,N,N);
    }
    MPI_Finalize();
    free2dint(&A);
    free2dint(&B);
    free2dint(&C);
    free2dint(&D);

    return 0;
}
#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(){
  int rank,size, nproc, ierr;
  int N = 3;

  MPI_Comm comm;

  MPI_Init(NULL,NULL);
              
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

   // If the number of processes assigned is greater than N then create a new MPI_comm with only N of the processes.
   if(nproc > N){

      // Creates a gorup including all of the assigned processes
      MPI_Group world_group;
      MPI_Comm_group(MPI_COMM_WORLD, &world_group);
      
      // Want to include procceses of rank 0,..,N-1 from MPI_COMM_WORLD in the new sub group.
      int ranks[N];

      for(int i=0;i<N;i++){ ranks[i] = i;}

      // Create a subset of N of the given number of processes and then create the communicator comm using that group.
      MPI_Group subset_group;
      MPI_Group_incl(world_group, N, ranks, &subset_group);
      MPI_Comm_create_group(MPI_COMM_WORLD, subset_group, 0 , &comm);
   }

   // The communicator comm should now have <= N processes, get it's ranks and size.
   comm = MPI_COMM_WORLD;
   MPI_Comm_rank(comm, &rank);
   MPI_Comm_size(comm, &size);

  int B[N][N]; // Initialises the matrix B for all processes.

   // Constructs the matrix B only on the process of rank 0.
  if (rank == 0){
      for (int i = 1; i < N+1 ; i++){
         for (int j = 1; j < N+1; j++){
            B[i-1][j-1] = (j+i)*(N-j+1);
         }
      }
  }
   // Broadcast the matrix B to the other processes.
   MPI_Bcast(B, N*N, MPI_INT, 0 , comm);

   //Initialise a vector of length 3 to store the individual rows of A.
   int a_row[N];

   // Calculate the row of A, using i = rank + 1. I.e. the zeroth process constructs the first row, the first process constructs the second row etc.
   for(int j = 1; j < N+1;j++){
      a_row[j-1] = (N-j+(rank+1)+1)*(rank+1);
   }

   // Initialise the vector to store the result of multiplying a row of A with B.
   int c[N];

   // Multiply the row of a with the matrix B.
   for(int i=0;i<N;i++){
      int sum = 0;
      for(int j=0; j< N;j++){
         sum += a_row[j]*B[j][i];
      }
      c[i] = sum;
   }

   int globaldata[N*N];

   MPI_Gather(c, N, MPI_INT, globaldata, N, MPI_INT, 0, comm);

   // Construct the final matrix from the list of elements produced from the gather command.
   if(rank == 0){
      int final_matrix[N][N];
      for(int i =0; i<N;i++){
         for(int j=0; j<N; j++){
            final_matrix[i][j] = globaldata[i*N+j];
         }
      }

   // Print the resulting matrix to the output file.
   for (int i =0;i<N; i++){
      for(int j = 0;j<N;j++){
         cout<< final_matrix[i][j]<< " ";
         }
      cout<< endl;
      }
   }

   MPI_Finalize();

}

#include <iostream>
#include <mpi.h>
using namespace std;
#define N 3

double A[N];
double B[N][N];
double C[N];
double D[N][N];

static void init_matrix(void){
int i, j;
for (i = 0; i < N; i++){
    for (j = 0; j < N; j++){
        B[i][j] = (j+i+2)*(N-j);  
                            }  
                        }  
} 

static void init_row(int i){
int  j;
    for (j = 0; j < N; j++){
        A[j] = (N-j+i+1)*(i+1);
                            }   
} 


static void print_matrix(double M[N][N]){
int i, j;
for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
        printf("%7.2d", int(M[i][j]));
                     }
        printf("\n");
        }  
}       

static void print_row(double M[N]){
int j;
    for(j = 0; j < N; j++){
        printf("%7.2d", int(M[j]));
                     }
        printf("\n");
}  

int main(){
    
    int rank, nproc;
    double start_time, end_time;
   
    // Start parallel sequence
    MPI_Init(NULL, NULL);
    MPI_Comm comm;
    comm  = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &nproc);
    MPI_Comm_rank(comm, &rank);

    init_matrix();
    start_time = MPI_Wtime();
    MPI_Bcast(B, N, MPI_DOUBLE, 0, comm); 
    MPI_Barrier(comm);
   
    init_row(rank);
    printf("#Check A row = %d, #Nodes = %d\n", rank, nproc);
    // Matrix (row-vector) multiplication
    print_row(A);
    for(int j=0; j<N; j++){
           C[j] = 0;
           for(int k=0; k<N; k++){
              C[j] += A[k]*B[k][j]; }
        }
    printf("#Check C row = %d, #Nodes = %d\n", rank, nproc);
    print_row(C);

  // Gather rows from individual processes
     MPI_Gather(C, N, MPI_DOUBLE, D, N, MPI_DOUBLE, 0, comm);
     end_time = MPI_Wtime(); 
     MPI_Barrier(comm);
     MPI_Finalize();

  // Display Gathered matrix and results
   if (rank == 0){
        printf("Brodcasted matrix B:");
        print_matrix(B);
        printf("Gathered matrix D:");
        print_matrix(D); 
        printf("Run time on %2d nodes: %f\n", nproc, end_time-start_time);   }

 
     return 0;
}
    

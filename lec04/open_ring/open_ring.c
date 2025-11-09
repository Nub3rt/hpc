#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int data;

  if (rank == 0) { // first chain
    data = 42;
    printf("Process %d starting chain, sending data = %d\n", rank, data);
    MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
  } else if (rank != world_size - 1) { // middle chains
    MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received data %d, sending it through...\n", rank, data);
    MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
  } else { // last chain
    MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d ending chain, data received = %d\n", rank, data);
  }

  MPI_Finalize();

  return 0;
} /* main */


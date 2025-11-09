#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int data;

  if (rank == 0) {
    data = 42;
    printf("Main process starting chain with data = %d\n", data);
    MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    data = -1;

    MPI_Recv(&data, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Main process closes chain, received data = %d\n", data);
  } else {
    MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received data = %d, sending through...\n", rank, data);
    MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
} /* main */


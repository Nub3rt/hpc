#include <stdio.h>
#include <mpi.h>

int reduce(int tag, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int data, sum = 0;
  for (int i = 0; i < size; ++i)
    if (i != rank) {
      MPI_Recv(&data, 1, MPI_INT, i, tag, comm, MPI_STATUS_IGNORE);
      sum += data;
    }

  return sum;
}

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int sum = reduce(0, MPI_COMM_WORLD);
    printf("Reduce result in main process: %d\n", sum);
  } else {
    printf("Process %d sending it's rank as data!\n", rank);
    MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
} /* main */


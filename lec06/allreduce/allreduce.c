#include <stdio.h>
#include <mpi.h>

int allreduce_node(int tag, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int data, sum = 0;
  for (int i = 0; i < size; ++i)
    if (i != rank) {
      MPI_Recv(&data, 1, MPI_INT, i, tag, comm, MPI_STATUS_IGNORE);
      sum += data;
    }

  for (int i = 0; i < size; ++i)
    if (i != rank)
      MPI_Send(&sum, 1, MPI_INT, i, tag, comm);

  return sum;
}

int allreduce_leaf(const int* buf, int dest, int tag, MPI_Comm comm, MPI_Status* status) {
  int res;
  MPI_Send(buf, 1, MPI_INT, dest, tag, comm);
  MPI_Recv(&res, 1, MPI_INT, dest, tag, comm, status);
  return res;
}

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int sum = allreduce_node(0, MPI_COMM_WORLD);
    printf("AllReduce result in main process: %d\n", sum);
  } else {
    printf("Process %d sending it's rank as data!\n", rank);
    int sum = allreduce_leaf(&rank, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received sum data = %d\n", rank, sum);
  }

  MPI_Finalize();

  return 0;
} /* main */


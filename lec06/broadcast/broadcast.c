#include <stdio.h>
#include <mpi.h>

void broadcast(const void* buf, int count, MPI_Datatype datatype, int tag, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i < size; ++i)
    if (i != rank)
      MPI_Send(buf, count, datatype, i, tag, comm);
}

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 int data;

  if (rank == 0) {
    data = 42;
    printf("Main process broadcasting data = %d...\n", data);
    broadcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received data = %d\n", rank, data);
  } 

  MPI_Finalize();

  return 0;
} /* main */


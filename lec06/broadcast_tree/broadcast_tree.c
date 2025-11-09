#include <stdio.h>
#include <mpi.h>

void broadcast(void* buf, int count, MPI_Datatype datatype, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  MPI_Status status;
  int loops = 0;

  if (rank == 0) {
    int s = 1;
    while (s < size) {
      ++loops;
      s *= 2;
    }
  } else {
    MPI_Recv(buf, count, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
    loops = status.MPI_TAG;
  }
  
  while (loops != 0) {
    --loops;
    int dest = rank + (1 << loops);
    if (dest < size) {
//      printf("Process %2d -> process %2d\n", rank, dest);
      MPI_Send(buf, count, datatype, dest, loops, comm);
    }
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int data;
  if (rank == 0)
    data = 42;

  broadcast(&data, 1, MPI_INT, MPI_COMM_WORLD);
  printf("Process %2d: %d\n", rank, data);

  MPI_Finalize();

  return 0;
} /* main */


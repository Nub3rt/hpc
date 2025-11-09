#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  char msg[5];

  if (rank == 0) {
    for (int i = 0; i < 5; ++i) {
      MPI_Send("ping", 5, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(msg, 5, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", msg);
    }
  }
  if (rank == 1) {
    for (int i = 0; i < 5; ++i) {
      MPI_Recv(msg, 5, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", msg);
      MPI_Send("pong", 5, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();

  return 0;
} /* main */


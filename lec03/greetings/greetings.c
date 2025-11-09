#include <stdio.h>
#include <mpi.h>
#include <string.h>

const int MAX_STRING = 100;

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  char greeting[MAX_STRING];

  if (rank != 0) {
    sprintf(greeting, "Greetings from process %d of %d!", rank, world_size);
    MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    printf("Greetings from main process (%d)! Num of processes: %d\n", rank, world_size);
    for (int i = 1; i < world_size; ++i) {
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", greeting);
    }
  } 

  MPI_Finalize();

  return 0;
} /* main */


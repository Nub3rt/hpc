#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int runs = 21;
  int *buff = malloc((1 << (runs - 1)) * sizeof(int));

  if (rank == 0)
  printf("%-8s%-16s%-16s\n", "n", "time (sec)", "rate (MB/sec)");

  for (int i = 0; i < runs; ++i) {
    int n = 1 << i;
    if (rank == 0) {
      double start = MPI_Wtime();
      MPI_Send(buff, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(buff, n, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      double end = MPI_Wtime();
      double diff = end - start;
      double mbps = n / (1024.0 * 1024.0) / sizeof(int) * 8.0 / diff;
      printf("%-8d%-16.6lf%-16.6lf\n", n, diff, mbps);
    } else {
      MPI_Recv(buff, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(buff, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  free(buff);

  MPI_Finalize();

  return 0;
} /* main */


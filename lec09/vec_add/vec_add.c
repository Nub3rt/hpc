#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void parallel_vector_sum(
    double local_x[],
    double local_y[],
    double local_z[],
    int    local_n) {
  for (int i = 0; i < local_n; ++i)
    local_z[i] = local_x[i] + local_y[i];
} /* parallel_vector_sum */

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n;
  double *x = NULL, *y = NULL, *z = NULL;
  
  if (rank == 0) {
    scanf("%d", &n);
    x = malloc(n * sizeof(double));
    y = malloc(n * sizeof(double));
    z = malloc(n * sizeof(double));
    for (int i = 0; i < n; ++i)
      scanf("%lf", &x[i]);
    for (int i = 0; i < n; ++i)
      scanf("%lf", &y[i]);
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int local_n = n / size;

  double *local_x, *local_y, *local_z;
  local_x = malloc(local_n * sizeof(double));
  local_y = malloc(local_n * sizeof(double));
  local_z = malloc(local_n * sizeof(double));

  MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(y, local_n, MPI_DOUBLE, local_y, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = 0; i < local_n; ++i)
    local_z[i] = local_x[i] + local_y[i];

  MPI_Gather(local_z, local_n, MPI_DOUBLE, z, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  free(local_x);
  free(local_y);
  free(local_z);

  if (rank == 0) {
    printf("Main process, vectors added:\n%.2lf", z[0]);
    for (int i = 1; i < n; ++i)
      printf(" %.2lf", z[i]);
    printf("\n");

    free(x);
    free(y);
    free(z);
  }

  MPI_Finalize();

  return 0;
} /* main */


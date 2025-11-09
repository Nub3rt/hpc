#include <stdio.h>
#include <mpi.h>

double f(double x) { return x * x; }

double trap(
    double left_endpt,
    double right_endpt,
    int    trap_count,
    double base_len) {
  double estimate;

  estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
  for (int i = 1; i <= trap_count; ++i)
    estimate += f(left_endpt + i * base_len);
  estimate *= base_len;

  return estimate;
} /* trap */

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n, local_n;
  double a, b, h, local_a, local_b;
  double local_int, total_int;

  if (rank == 0)
    scanf("%lf %lf %d", &a, &b, &n);

  MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n, 1, MPI_INT   , 0, MPI_COMM_WORLD);

  h = (b - a) / n;
  local_n = n / size;

  local_a = a + rank * local_n * h;
  local_b = local_a + local_n * h;
  local_int = trap(local_a, local_b, local_n, h);

  MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    printf("With n = %d trapezoids, our estimate\nof the integral from %f to %f = %.4f\n", n, a, b, total_int);

  MPI_Finalize();

  return 0;
} /* main */


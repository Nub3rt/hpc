#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x) { return x * x; }

double trap(double a, double b, int n) {
  int rank = omp_get_thread_num();
  int size = omp_get_num_threads();
  double h = (b - a) / n;
  int local_n = n / size;
  double local_a = a + rank * local_n * h;
  double local_b = local_a + local_n * h;

  double result = (f(local_a) + f(local_b)) / 2.0;
  for (int i = 1; i <= local_n - 1; ++i)
    result += f(local_a + i * h);
  result *= h;

  return result;
} /* trap */

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Missing required argument [thread_count]\n");
    exit(1);
  }

  double a, b, total_int = 0.0;
  int n, thread_count = strtol(argv[1], NULL, 10);

  scanf("%lf %lf %d", &a, &b, &n);

# pragma omp parallel num_threads(thread_count) reduction(+: total_int)
  total_int += trap(a, b, n);

  printf("With n = %d trapezoids, our estimate\n of the integral from %.2lf to %.2lf = %.2lf\n", n, a, b, total_int);

  return 0;
} /* main */


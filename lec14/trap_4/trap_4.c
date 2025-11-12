#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x) { return x * x; }

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Missing required argument [thread_count]\n");
    exit(1);
  }

  double a, b;
  int n, thread_count = strtol(argv[1], NULL, 10);

  scanf("%lf %lf %d", &a, &b, &n);

  double h = (b - a) / n;
  double total_int = (f(a) + f(b)) / 2.0;
# pragma omp parallel for num_threads(thread_count) reduction(+: total_int)
  for (int i = 1; i <= n - 1; ++i)
    total_int += f(a + i * h);
  total_int *= h;

  printf("With n = %d trapezoids, our estimate\n of the integral from %.2lf to %.2lf = %.2lf\n", n, a, b, total_int);

  return 0;
} /* main */


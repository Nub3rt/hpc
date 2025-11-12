#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Missing required argument [thread_count]\n");
    exit(1);
  }

  int thread_count = strtol(argv[1], NULL, 10);
  int n;
  scanf("%d", &n);

//  double factor = 1.0;
//  double sum = 0.0;
//  for (int k = 0; k < n; ++k) {
//    sum += factor / (2 * k + 1);
//   factor = -factor;
//  }
//  sum *= 4.0;

  double sum = 0.0;
# pragma omp parallel for num_threads(thread_count) reduction(+:sum)
  for (int k = 0; k < n; ++k) {
    double factor = k % 2 == 0 ? 1.0 : -1.0;
    sum += factor / (2 * k + 1);
  }
  sum *= 4.0;

  printf("Pi estimate with %d iterations: %lf\n", n, sum);

  return 0;
} /* main */


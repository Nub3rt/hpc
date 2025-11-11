#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello() {
  int rank = omp_get_thread_num();
  int size = omp_get_num_threads();

  printf("Hello world from thread %d of %d\n", rank, size);
} /* hello */

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Missing argument [thread_count]\n");
    exit(1);
  }
  int thread_count = strtol(argv[1], NULL, 10);

# pragma omp parallel num_threads(thread_count)
  hello();

  return 0;
} /* main */


#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {

  printf("Hello world from the serial world!\n");
  
# pragma omp parallel num_threads(10)
  {
    printf("Hello world from thread = %d!\n", omp_get_thread_num());
  }

  printf("Back to the serial world\n");

  return 0;
} /* main */


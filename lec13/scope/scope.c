#include <stdio.h>
#include <omp.h>

void foo(int* pn) {
  printf("*pn = %d, pn = %p, &pn = %p\n", *pn, pn, &pn);
} /* foo */

int main(int argc, char* argv[]) {

 int n = 42;
	
# pragma omp parallel num_threads(2)
  foo(&n);

# pragma omp parallel num_threads(2)
  {
    int m = 42;
    printf("n = %d, n& = %p\n", n, &n);
    printf("m = %d, m& = %p\n", m, &m);
  }

  return 0;
} /* main */


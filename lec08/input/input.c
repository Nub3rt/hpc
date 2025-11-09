#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  int a, b, c;
  scanf("%d %d %d", &a, &b, &c);
  printf("%d %d %d %d\n", c, b, a, a + b + c);

  return 0;
} /* main */


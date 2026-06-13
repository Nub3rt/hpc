#include <stdio.h>
#include <mpi.h>
#include <vector>

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> v;
  for (int i = 0; i <= rank; ++i)
    v.push_back(i);
  printf("Process %i vector size: %li\n", rank, v.size());

  MPI_Finalize();

  return 0;
} /* main */


#include "mpi/mpiHelper.h"

int main(int argc, char **argv) {

  // here launch parallel environment
  // Call proxy function from MPI Helper, which makes mpi object
  // globally available.
  initMPI(argc, argv);

  // Print parallelization info
  parallelInfo();

  deleteMPI();
  return (0);
}

#include "mpi/mpiHelper.h"
#include "PMatrix.h"
#include "example2.h"
#include "example3.h"
#include <chrono>

int main(int argc, char **argv) {

  // here launch parallel environment
  // Call proxy function from MPI Helper, which makes mpi object
  // globally available.
  initMPI(argc, argv);

  // Print parallelization info
  parallelInfo();

  // --------------------- Example 2 ------------------------------

  //example2();

  // --------------------- Example 3 ------------------------------

  //example3();

  // close out MPI env ---------------------------------------------------------

  deleteMPI();
  return (0);
}

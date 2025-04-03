#pragma once
#include "PMatrix.h"

void example3() {

  // --------------------- Example 3------------------------------

  int dim = 1024*8;
  int nBlocks = int(dim/64); // block size of 64

  // allocate the matrix
  ParallelMatrix<double> pmat = ParallelMatrix<double>(dim, dim, nBlocks, nBlocks);

  // fill in the matrix, iterate over local rows and columns
  for(auto [rowIdx,colIdx] : pmat.getAllLocalElements()) {
    // fill with nonsense values (for now, use MPI rank)
    pmat(rowIdx, colIdx) = mpi->getRank();
  }
  if(mpi->mpiHead()) std::cout << "Done filling matrix." << std::endl;

  // diagonalize
  auto start = std::chrono::high_resolution_clock::now();
  auto [eigenvalues, pEigenvectors] = pmat.diagonalize();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  if(mpi->mpiHead()) std::cout << "Time [milli s]: " << duration.count() << std::endl;

} // end function

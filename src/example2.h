#pragma once
#include "mpi/mpiHelper.h"
#include "PMatrix.h"

 // --------------------- Example 2 ------------------------------

void example2() {

  int nRows = 8;
  int nCols = 8;

  int nBlocksRows = 0; // Change these!
  int nBlocksCols = 0;

  ParallelMatrix<double> theMatrix = ParallelMatrix<double>(nRows, nCols, nBlocksRows, nBlocksCols);
  Eigen::MatrixXi serialMatrix(nRows, nCols); serialMatrix.setZero();

  // iterate over rows and columns
  for(auto [rowIdx,colIdx] : theMatrix.getAllLocalElements()) {

    // fill a copy of this serial matrix with the rank of the process that owns the parallel matrix element
    serialMatrix(rowIdx, colIdx) = mpi->getRank();

  }
  // reduce the serialMatrix owned by each MPI process into one matrix, which is basically
  // a print of the ranks owning elements of the parallel one
  mpi->allReduceSum(&serialMatrix);

  if(mpi->mpiHead()) std::cout << "the matrix: \n" << serialMatrix << std::endl;

}

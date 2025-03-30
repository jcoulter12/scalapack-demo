#include "mpi/mpiHelper.h"
#include "PMatrix.h"
#include <chrono>

int main(int argc, char **argv) {

  // here launch parallel environment
  // Call proxy function from MPI Helper, which makes mpi object
  // globally available.
  initMPI(argc, argv);

  // Print parallelization info
  parallelInfo();
  
  // HERE UTILIZE MATRICES -------------------------------------------
  
  int numRows = 2; 
  int numCols = 2; 

  // we do not specify numBlocksRows/Cols, 
  // as this matrix will have to be distributed using 
  // a square process grid. Not specifying them 
  // activates the default use case for a square process grid
  ParallelMatrix<double> pMat(numRows,numCols);
  
  // iterate over rows and columns 
  for(auto [rowIdx,colIdx] : pMat.getAllLocalElements()) {
    
    // fill with nonsense values (for now, use MPI rank)
    pMat(rowIdx, colIdx) = mpi->getRank(); 
    
  }
  auto [eigenvalues, pEigenvectors] = pMat.diagonalize();
    
    
  // --------------------- BLOCKSIZE TEST ------------------------------
  
  //int matSize = numRows * numCols; 
  //int nBlocks = int(matSize/64.);
  // TODO Should we move this to PMatrix constructor instead?
  // seems tiny number of blocks is a problem, but it should
  // only come up for very tiny cases where speed is not an issue,
  // therefore, we default to 4 blocks if this happens.
  // Seems like this is maybe a bug in scalapack?
  //if(nBlocks <= int(sqrt(mpi->getSize()))) nBlocks = int(sqrt(mpi->getSize())) * 2;

  int dim = 1024*8; 

  for(int nBlocks = 16; nBlocks <= dim; nBlocks=nBlocks+16){
    
    ParallelMatrix<double> theMatrix = ParallelMatrix<double>(dim, dim, 0, 0, nBlocks, nBlocks);
    
    auto start = std::chrono::high_resolution_clock::now();
    theMatrix.diagonalize();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if(mpi->mpiHead()) std::cout << "blockSize " << int(dim/nBlocks) << " Time [s]: " << duration.count() << std::endl;
  }

  // -------------------------------------------------------------------

  deleteMPI();
  return (0);
}


// Figure out how to print? 
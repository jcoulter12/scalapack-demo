#pragma once 

#include "mpi/mpiHelper.h"
#include <iostream>
#include <string>


inline void Error(const std::string &errMessage, const int &errCode = 1) {
  if (errCode != 0) {
    if (mpi->mpiHead()) {
      std::cout << "\nError!" << std::endl;
      std::cout << errMessage << "\n" << std::endl;
    }
    mpi->barrier();
    mpi->finalize();
    exit(errCode);
  }
}

inline void DeveloperError(const std::string &errMessage, const int &errCode = 1) {
  if (errCode != 0) {
    if (mpi->mpiHead()) {
      std::cout << "\nDeveloper Error:" << std::endl;
      std::cout << errMessage << "\n" << std::endl;
    }
    mpi->barrier();
    mpi->finalize();
    exit(errCode);
  }
}
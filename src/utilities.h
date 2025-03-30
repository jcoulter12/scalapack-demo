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

// A function to allocate a dynamically sized array. It tricks the
// compiler into thinking the size is a constant via the const identifier
// on the argument. This resolves issues with VLAs -- see crystal.cpp
template <typename T> T* allocate(T *&array, const unsigned int size){
        array = new T [size];
        return array;
}

inline int mod(const int &a, const int &b) { return (a % b + b) % b; }
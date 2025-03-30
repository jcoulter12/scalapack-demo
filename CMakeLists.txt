# set cmake version
cmake_minimum_required(VERSION 3.16)

# set cpp standard flags
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

# define the project 
project(PMatrix VERSION 1.0 LANGUAGES CXX)

############ OPTIONS ################
# these can be toggled in a build script by
# supplying arguments to cmake like cmake -DMPI_AVAIL=OFF ../
option(MPI_AVAIL "Build with MPI wrappers" ON)
option(OMP_AVAIL "Build with OMP" OFF)
add_definitions("-DMPI_AVAIL") 

############### SOURCE ###############

FILE(GLOB_RECURSE SOURCE_FILES src src/*.cpp mpi/*.cpp)
FILE(GLOB TEST_SOURCES test/*.cpp)

include_directories(src)
include(GoogleTest)

add_executable(PMatrix src/main.h ${SOURCE_FILES})
add_executable(tests ${TEST_SOURCES} ${SOURCE_FILES})
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)

enable_testing()
gtest_discover_tests(
    tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

################ MPI ####################

find_package(MPI REQUIRED)
target_link_libraries(PMatrix MPI::MPI_CXX)
target_link_libraries(tests MPI::MPI_CXX)

################ SCALAPACK ####################

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
include_directories(${BLAS_INCLUDE_DIR})
include_directories(${LAPACK_INCLUDE_DIR})

find_library(SCALAPACK_LIB NAMES scalapack scalapack-openmpi mkl_scalapack_lp64 PATHS ENV LD_LIBRARY_PATH)
if(${SCALAPACK_LIB} MATCHES mkl)
    if("${MPI_CXX_LIBRARIES}" MATCHES openmpi)
        find_library(BLACS_LIB NAMES mkl_blacs_openmpi_lp64 PATHS ENV LD_LIBRARY_PATH)
    elseif("${MPI_CXX_LIBRARIES}" MATCHES intel)
        find_library(BLACS_LIB NAMES mkl_blacs_intelmpi_lp64 PATHS ENV LD_LIBRARY_PATH)
    else()
        find_library(BLACS_LIB NAMES mkl_blacs_intelmpi_lp64 PATHS ENV LD_LIBRARY_PATH)
        #message(FATAL_ERROR "Confused by MPI library when looking for BLACS.")
    endif()
    if(${BLACS_LIB} MATCHES NOTFOUND)
        message(FATAL_ERROR "Found Intel SCALAPACK but not BLACS")
    endif()
    message(STATUS "Found BLACS: ${BLACS_LIB}")
endif()
message(STATUS "Found SCALAPACK: ${SCALAPACK_LIB}")

# Important to link scalapack before blas and lapack
target_link_libraries(PMatrix ${SCALAPACK_LIB} ${BLACS_LIB} ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES} )
target_link_libraries(tests ${SCALAPACK_LIB} ${BLACS_LIB} ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES} )

find_package (Eigen3 3.3 REQUIRED)
message(STATUS "Found Eigen: ${Eigen3_VERSION} ${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})
target_link_libraries(PMatrix Eigen3::Eigen)
target_link_libraries(tests Eigen3::Eigen)

# TODO delete these extras 

# build with openMP
#if(OMP_AVAIL)
#  find_package(OpenMP)
#  target_link_libraries(phoebe OpenMP::OpenMP_CXX)
#  target_link_libraries(runTests OpenMP::OpenMP_CXX)
#  add_definitions("-DOMP_AVAIL")
#endif()

############### ELPA #################
#if(ELPA_AVAIL)
#  find_library(ELPA_LIB NAMES libelpa_openmp.so PATHS "${ELPA_DIR}/lib/")
#  message(STATUS "Found ELPA: ${ELPA_LIB}")
#  include_directories("/n/holystore01/LABS/kozinsky_lab/Lab/User/jcoulter/codes/elpa/build/include/elpa_openmp-2023.05.001/")
#  target_link_libraries(phoebe ${ELPA_LIB})
#  target_link_libraries(runTests ${ELPA_LIB})
#  add_definitions("-DELPA_AVAIL")
#endif()
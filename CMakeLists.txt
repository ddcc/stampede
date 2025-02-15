cmake_minimum_required(VERSION 2.8.8)
set(USE_HTM OFF CACHE BOOL "Use hardware transactional memory")
set(USE_STM OFF CACHE BOOL "Use software transactional memory")
set(USE_XTM OFF CACHE BOOL "Use exclusive software transactional memory")
set(USE_XTM_INLINE OFF CACHE BOOL "Use exclusive software transactional memory")
set(USE_SALLOC OFF CACHE BOOL "Use scalable memory allocator")
set(USE_PAPI OFF CACHE BOOL "Use PAPI" FORCE)
set(USE_TINYSTM OFF CACHE BOOL "Use Galois+tinySTM")
set(USE_STAMP_PALLOC OFF CACHE BOOL "Use Stamp parallel malloc")
set(USE_NO_WORKSTEALING OFF CACHE BOOL "Disable workstealing")
set(USE_ABORT_POLICY_BOUNDED OFF CACHE BOOL "")
set(USE_ABORT_POLICY_EAGER OFF CACHE BOOL "")
set(USE_ABORT_POLICY_DOUBLE OFF CACHE BOOL "")
set(USE_HWTOPO ON CACHE BOOL "Use Galois Hardware topology-based mapping of threads")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(ExternalProject)

set(GaloisExtraArgs)
if(CMAKE_CXX_COMPILER_ID MATCHES "XL")
  set(GaloisExtraArgs "-C ${source_dir}/cmake/Toolchain/ppc64-bgq-linux-tryrunresults.cmake
      -DCMAKE_TOOLCHAIN_FILE=${PROJECT_SOURCE_DIR}/Galois-2.2.1/cmake/Toolchain/ppc64-bgq-linux-xlc.cmake")
endif()

if(USE_TINYSTM)
  ExternalProject_Add(tinystm
    PREFIX tinystm
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/tinySTM-1.0.5
    CMAKE_ARGS 
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/tinystm)

  # Force check of tinySTM changes for every make
  ExternalProject_Add_Step(tinystm forceconfigure
      DEPENDEES update
      DEPENDERS configure
      ALWAYS 1)

  ExternalProject_Get_Property(tinystm install_dir)
  set(tinystm_install_dir ${install_dir})

  set(GALOIS_DEPS "tinystm")
  set(TINYSTM_CMAKE_EXPORT "${tinystm_install_dir}/cmake/tinystm")
elseif(USE_XTM)
  ExternalProject_Add(xtm
    PREFIX xtm
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/benchmarks/xtm
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/xtm
      -DUSE_XTM_INLINE=${USE_XTM_INLINE})

  # Force check of tinySTM changes for every make
  ExternalProject_Add_Step(xtm forceconfigure
      DEPENDEES update
      DEPENDERS configure
      ALWAYS 1)

  ExternalProject_Get_Property(xtm install_dir)
  set(xtm_install_dir ${install_dir})

  set(GALOIS_DEPS "xtm")
  set(TINYSTM_CMAKE_EXPORT "${xtm_install_dir}/cmake/xtm")
endif()

ExternalProject_Add(Galois
  DEPENDS ${GALOIS_DEPS}
  PREFIX Galois
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/Galois-2.2.1
  CMAKE_ARGS
    ${GaloisExtraArgs}
    -Wno-dev
    -DTINYSTM_CMAKE_EXPORT=${TINYSTM_CMAKE_EXPORT}
    -DSKIP_COMPILE_APPS=1
    -DUSE_HWTOPO=${USE_HWTOPO}
    -DUSE_LONGJMP=1
    -DUSE_PAPI=0
    -DUSE_NO_WORKSTEALING=${USE_NO_WORKSTEALING}
    -DUSE_ABORT_POLICY_BOUNDED=${USE_ABORT_POLICY_BOUNDED}
    -DUSE_ABORT_POLICY_EAGER=${USE_ABORT_POLICY_EAGER}
    -DUSE_ABORT_POLICY_DOUBLE=${USE_ABORT_POLICY_DOUBLE}
    -DUSE_TINYSTM=${USE_TINYSTM}
    -DUSE_TINYSTM_XTM=${USE_XTM}
    -DUSE_HTM=${USE_HTM}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/Galois
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

# Force check of tinySTM changes for every make
ExternalProject_Add_Step(Galois forceconfigure
    DEPENDEES update
    DEPENDERS configure
    ALWAYS 1)

ExternalProject_Get_Property(Galois install_dir)
set(galois_install_dir ${install_dir})

ExternalProject_Add(benchmarks
  DEPENDS ${GALOIS_DEPS} Galois
  PREFIX benchmarks
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/benchmarks
  CMAKE_ARGS
    -DGALOIS_CMAKE_EXPORT=${galois_install_dir}/cmake/Galois
    -DTINYSTM_CMAKE_EXPORT=${TINYSTM_CMAKE_EXPORT}
    -DUSE_HTM=${USE_HTM}
    -DUSE_STM=${USE_STM}
    -DUSE_XTM=${USE_XTM}
    -DUSE_XTM_INLINE=${USE_XTM_INLINE}
    -DUSE_SALLOC=${USE_SALLOC}
    -DSALLOC_LIB=${SALLOC_LIB}
    -DUSE_PAPI=${USE_PAPI}
    -DUSE_STAMP_PALLOC=${USE_STAMP_PALLOC}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/benchmarks
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

ExternalProject_Add_Step(benchmarks forceconfigure
    DEPENDEES update
    DEPENDERS configure
    ALWAYS 1)

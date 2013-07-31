
set(LIB_NAME palgorithm)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED)
find_package(openmp)

include_directories(${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

set(LIB_SOURCES

    )

set(LIB_HEADERS
    pqsort.hpp
    ts_queue.hpp
)

set(TEST_SRC
    tests/RunAllTests.cpp
    tests/palgorithm.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/src)

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS} TEST_SOURCES ${TEST_SRC})

#test targets
addSourceFlags(qsort.cpp COMPILE_FLAGS ${OpenMP_CXX_FLAGS})
add_executable(qsort_tester qsort.cpp)
addFlags(qsort_tester LINK_FLAGS ${OpenMP_CXX_FLAGS})
turnOnCpp11(qsort_tester)

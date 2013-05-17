
set(LIB_NAME palgorithm)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(LIB_SOURCES

    )

set(LIB_HEADERS
    pqsort.hpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS} TEST_SOURCES qsort.cpp)

############################### tests ###############################

#registerTest(${LIB_NAME} tests/RunAllTests.cpp tests/utils_tests.cpp)


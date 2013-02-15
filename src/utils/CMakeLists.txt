
set(LIB_NAME utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(LIB_SOURCES
    cpuinfo.cpp
    )

set(LIB_HEADERS
    data_ptr.hpp
    cpuinfo.hpp
)

set(TEST_SOURCES
    tests/RunAllTests.cpp
    tests/utils_tests.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/src)

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS} TEST_SOURCES ${TEST_SOURCES})

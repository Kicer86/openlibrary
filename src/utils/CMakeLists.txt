
set(LIB_NAME utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)

set(LIB_SOURCES
    cpuinfo.cpp
    )

set(LIB_HEADERS
    anonymous_ptr.hpp
    data_ptr.hpp
    cpuinfo.hpp
    optional.hpp
)

set(TEST_SRC
    tests/RunAllTests.cpp
    tests/utils_tests.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/src)

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS} TEST_SOURCES ${TEST_SRC})

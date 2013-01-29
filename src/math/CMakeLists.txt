
set(LIB_NAME math)

project(${LIB_NAME})

cmake_minimum_required( VERSION 2.6 )

set(MATH_SRC
  crc32.cpp
  math.cpp
)

set(MATH_HEADERS
  crc32.hpp
  math.hpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)
register_library(${LIB_NAME} SOURCES ${MATH_SRC} HEADERS ${MATH_HEADERS})

############################### tests ###############################

registerTest(math tests/RunAllTests.cpp tests/math_tests.cpp)

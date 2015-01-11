
project(math)

cmake_minimum_required( VERSION 2.6 )

set(MATH_SRC
  crc32.cpp
  math.cpp
)

set(MATH_HEADERS
  crc32.hpp
  math.hpp
)

set(TEST_SRC
    tests/RunAllTests.cpp
    tests/math_tests.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(math SHARED ${MATH_SRC} ${MATH_HEADERS})
register_target(NAME math SOURCES ${MATH_SRC} HEADERS ${MATH_HEADERS})

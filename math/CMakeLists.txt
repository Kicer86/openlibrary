
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

register_library(math SOURCES ${MATH_SRC} HEADERS ${MATH_HEADERS})

############################### tests ###############################

#include(${CMAKE_SOURCE_DIR}/libraries.cmake)

#enable_testing()

#add_executable(TestTypes tests/types_test.cpp)
#target_link_libraries(TestTypes math ${UTILS_LIBRARIES})

#add_test(test_types ${CMAKE_CURRENT_BINARY_DIR}/TestTypes TestingTypes)

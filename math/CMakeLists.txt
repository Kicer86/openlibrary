
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

include_directories(${CMAKE_SOURCE_DIR})
register_library(math SOURCES ${MATH_SRC} HEADERS ${MATH_HEADERS})

############################### tests ###############################

find_package(CppUTest)

if(CPPUTEST_FOUND)
    message("math: adding tests - CppUTest package found")
    include_directories(${CPPUTEST_INCLUDE_DIRS})
    add_executable(mathTests
                   tests/RunAllTests.cpp tests/math_tests.cpp)

    target_link_libraries(mathTests ${CPPUTEST_LIBRARIES})

endif(CPPUTEST_FOUND)

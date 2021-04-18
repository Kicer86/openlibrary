
project( utils )
cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)

set(LIB_SOURCES
)

set(LIB_HEADERS
    data_ptr.hpp
)

set(TEST_SRC
    tests/RunAllTests.cpp
    tests/utils_tests.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/src)

#add_library(utils SHARED ${LIB_SOURCES} ${LIB_HEADERS})

register_target(NAME utils
                SOURCES ${LIB_SOURCES}
                HEADERS ${LIB_HEADERS})

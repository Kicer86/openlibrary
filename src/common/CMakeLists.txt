
project( common )
cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src )

set(LIB_SOURCES
    str_op.cpp
    )

set(LIB_HEADERS
    debug.hpp
    str_op.hpp
)

set(TEST_SRC
    )

add_library(common SHARED ${LIB_SOURCES} ${LIB_HEADERS})

register_target(NAME common SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

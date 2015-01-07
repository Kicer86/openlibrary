
project(putils)
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src )

set(LIB_SOURCES
    ts_resource.cpp
    )

set(LIB_HEADERS
    ts_queue.hpp
    ts_resource.hpp
)

set(TEST_SRC
    )

add_library(putils SHARED ${LIB_SOURCES} ${LIB_HEADERS})

register_target(NAME putils SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

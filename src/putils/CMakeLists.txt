
set(LIB_NAME putils)

project( ${LIB_NAME} )
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

add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_HEADERS})
turnOnCpp11(${LIB_NAME})

register_target(NAME ${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

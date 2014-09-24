
project( common )
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src )

set(LIB_SOURCES
    str_op.cpp
    )

set(LIB_HEADERS
    debug.hpp 
    std_macros.hpp 
    str_op.hpp 
    open_library_settings.hpp
)

set(TEST_SRC
    )

add_library(common SHARED ${LIB_SOURCES} ${LIB_HEADERS})
turnOnCpp11(common)

register_target(NAME common SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

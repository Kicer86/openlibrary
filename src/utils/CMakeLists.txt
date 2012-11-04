
set(LIB_NAME utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

set(LIB_SOURCES
    cpuinfo.cpp
    )

set(LIB_HEADERS
    copy_ptr.hpp
    cpuinfo.hpp
)

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

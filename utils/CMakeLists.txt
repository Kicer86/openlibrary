
set(LIB_NAME utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)


set(LIB_HEADERS
  copy_ptr.hpp
)

register_library(${LIB_NAME} HEADERS ${LIB_HEADERS})

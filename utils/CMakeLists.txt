set(LIB_NAME Utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)


set(LIB_HEADERS
  copy_ptr.hpp
)

install (FILES ${LIB_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

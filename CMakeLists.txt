project(library)
cmake_minimum_required(VERSION 2.8)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG=1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_subdirectory(htmlParser)
add_subdirectory(qextserialport)

set(LIBRARY_HEADERS debug.hpp std_macros.hpp str_op.hpp)

install(FILES ${LIBRARY_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

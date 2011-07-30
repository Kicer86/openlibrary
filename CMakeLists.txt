project(library)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(htmlParser)
add_subdirectory(qextserialport)

include (cmake/ForceAddFlags.cmake)

set(LIBRARY_HEADERS debug.hpp std_macros.hpp str_op.hpp)

install(FILES ${LIBRARY_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES cmake/ForceAddFlags.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/Modules)

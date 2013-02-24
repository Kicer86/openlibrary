
project(library)
cmake_minimum_required(VERSION 2.8)

include( ${PROJECT_SOURCE_DIR}/cmake/functions.cmake )
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#prepare target for running tests
add_custom_target(test)                    #all internal test targets should attach here
add_custom_target(valgrind_test)           #all valgrind tests should attach here
add_custom_target(test_code_coverage       #all code coeverage tests should attach here
                  DEPENDS test)

add_subdirectory(src/htmlParser htmlParser)
#add_subdirectory(qextserialport qextserialport)
add_subdirectory(src/router router)
add_subdirectory(src/QtExt QtExt)
add_subdirectory(src/utils utils)
add_subdirectory(src/math math)
add_subdirectory(src/palgorithms)

set(LIBRARY_HEADERS src/debug.hpp src/std_macros.hpp src/str_op.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
register_library(common HEADERS ${LIBRARY_HEADERS} SOURCES src/str_op.cpp)

getHeadersPath(HEADERS_INSTALL_PATH)
getHeadersBasePath(HEADERS_INSTALL_BASE_PATH)
set(OPENLIBRARY_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenLibrary)
configure_file(cmake/OpenLibrary.cmake.in "${PROJECT_BINARY_DIR}/OpenLibraryConfig.cmake" @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/OpenLibraryConfig.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})
install(FILES ${CMAKE_SOURCE_DIR}/cmake/publicFunctions.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})

#degug code
# get_cmake_property(vars VARIABLES)
#
# foreach(var ${vars})
#     message("${var} = ${${var}} ")
# endforeach()

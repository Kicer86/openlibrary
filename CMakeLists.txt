
project(openlibrary)
cmake_minimum_required(VERSION 2.8)

set(OPENLIBRARY_MAJOR_VERSION 1)
set(OPENLIBRARY_MINOR_VERSION 1)
set(OPENLIBRARY_PATCH_VERSION 0)
set(OPENLIBRARY_VERSION
  ${OPENLIBRARY_MAJOR_VERSION}.${OPENLIBRARY_MINOR_VERSION}.${OPENLIBRARY_PATCH_VERSION})
set(OPENLIBRARY_NAMESPACE OpenLibrary_)

#prepare paths
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/OpenLibrary)
endif()

include(GenerateExportHeader)
include(${PROJECT_SOURCE_DIR}/cmake/functions.cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

enable_testing()

#prepare target for running tests
add_custom_target(valgrind_test)           #all valgrind tests should attach here

add_library_path(src/htmlParser htmlParser)
add_library_path(qextserialport qextserialport EXPERIMENTAL)
add_library_path(src/router router EXPERIMENTAL)
add_library_path(src/QtExt QtExt)
add_library_path(src/utils utils)
add_library_path(src/math math EXPERIMENTAL)
add_library_path(src/palgorithm palgorithm)

set(LIBRARY_HEADERS src/debug.hpp src/std_macros.hpp src/str_op.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR})
register_library(common HEADERS ${LIBRARY_HEADERS} SOURCES src/str_op.cpp)

getHeadersPath(HEADERS_INSTALL_PATH)
getHeadersBasePath(HEADERS_INSTALL_BASE_PATH)
set(OPENLIBRARY_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/${DEF_INSTALL_CMAKE_DIR})

configure_file(cmake/OpenLibraryConfig.cmake.in "${PROJECT_BINARY_DIR}/OpenLibraryConfig.cmake" @ONLY)
configure_file(cmake/OpenLibraryConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/OpenLibraryConfigVersion.cmake" @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/OpenLibraryConfig.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})
install(FILES ${PROJECT_BINARY_DIR}/OpenLibraryConfigVersion.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})
install(FILES ${CMAKE_SOURCE_DIR}/cmake/publicFunctions.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})

#degug code
# get_cmake_property(vars VARIABLES)
#
# foreach(var ${vars})
#     message("${var} = ${${var}} ")
# endforeach()

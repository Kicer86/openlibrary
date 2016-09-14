
project(openlibrary)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

set(OPENLIBRARY_MAJOR_VERSION 2)
set(OPENLIBRARY_MINOR_VERSION 2)
set(OPENLIBRARY_PATCH_VERSION 0)
set(OPENLIBRARY_VERSION
  ${OPENLIBRARY_MAJOR_VERSION}.${OPENLIBRARY_MINOR_VERSION}.${OPENLIBRARY_PATCH_VERSION})
set(OPENLIBRARY_NAMESPACE OpenLibrary::)

option(BUILD_TESTS "Build tests. Set to ON to build tests")

#prepare paths
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/OpenLibrary)
endif()

include(GenerateExportHeader)
include(${PROJECT_SOURCE_DIR}/cmake/functions.cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(BUILD_TESTS)
    enable_testing()
endif()

add_library_path(src/htmlParser htmlParser EXPERIMENTAL)
#add_library_path(qextserialport qextserialport EXPERIMENTAL)
add_library_path(src/router router EXPERIMENTAL)
add_library_path(src/QtExt QtExt)
add_library_path(src/utils utils)
#add_library_path(src/math math EXPERIMENTAL)
#add_library_path(src/palgorithm palgorithm)
add_library_path(src/putils putils)
add_library_path(src/common common)

getHeadersPath(HEADERS_INSTALL_PATH)
getHeadersBasePath(HEADERS_INSTALL_BASE_PATH)
set(OPENLIBRARY_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/${DEF_INSTALL_CMAKE_DIR})

configure_file(cmake/OpenLibraryConfig.cmake.in "${PROJECT_BINARY_DIR}/OpenLibraryConfig.cmake" @ONLY)
configure_file(cmake/OpenLibraryConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/OpenLibraryConfigVersion.cmake" @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/OpenLibraryConfig.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})
install(FILES ${PROJECT_BINARY_DIR}/OpenLibraryConfigVersion.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})
install(FILES ${CMAKE_SOURCE_DIR}/cmake/OpenLibraryUse.cmake DESTINATION ${OPENLIBRARY_CMAKE_DIR})

#degug code
# get_cmake_property(vars VARIABLES)
#
# foreach(var ${vars})
#     message("${var} = ${${var}} ")
# endforeach()

if(BUILD_TESTS)
    add_subdirectory(tests)
    
    add_custom_target(RunUnitTests ALL
                      COMMAND ctest
                      DEPENDS putils_tests)
endif()

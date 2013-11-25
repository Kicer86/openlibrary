
project(gtest_runner)


cmake_minimum_required(VERSION 2.8.10)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(UNIX OR CYGWIN)
    add_definitions(-DOS_UNIX)
elseif(WIN32)
    add_definitions(-DOS_WIN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif(UNIX OR CYGWIN)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDES})

set(RUNNER_SOURCES
    main.cpp
   )

add_executable(gtest_runner ${RUNNER_SOURCES})
target_link_libraries(gtest_runner ${CMAKE_DL_LIBS} ${GTEST_LIBRARY})
turnOnCpp11(gtest_runner)
enableTestsAndCodeCoverage(gtest_runner)   #only link option, no sources. Without it, loaded library will not be instrumented

install(TARGETS gtest_runner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/OpenLibrary/bin)


#test lib
set(TEST_SOURCES
    so_main.cpp
   )

add_library(gtest_runner_test SHARED ${TEST_SOURCES})
target_link_libraries(gtest_runner_test ${GTEST_LIBRARY})
turnOnCpp11(gtest_runner_test)

install(TARGETS gtest_runner_test LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/OpenLibrary/bin)

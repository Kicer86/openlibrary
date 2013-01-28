
set(LIB_NAME utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(LIB_SOURCES
    cpuinfo.cpp
    )

set(LIB_HEADERS
    copy_ptr.hpp
    cpuinfo.hpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

############################### tests ###############################

find_package(CppUTest)

if(CPPUTEST_FOUND)
    message("utils: adding tests - CppUTest package found")
    include_directories(${CPPUTEST_INCLUDE_DIRS})
    add_executable(utilsTests
                   tests/RunAllTests.cpp tests/utils_tests.cpp)

    target_link_libraries(utilsTests ${CPPUTEST_LIBRARIES})

    add_custom_target(performUtilsCppUTests
                      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/utilsTests
                      DEPENDS utilsTests)

    turnOnCpp11(utilsTests)

    #attach performUtilsCppUTests to 'test' target
    add_dependencies(test performUtilsCppUTests)

endif(CPPUTEST_FOUND)

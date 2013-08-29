
set(LIB_NAME palgorithm)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED)
find_package(OpenMP)

add_subdirectory(bose-nelson)
generateswaptables(${CMAKE_CURRENT_BINARY_DIR} swap_tables swap_table_cpp swap_table_hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

set(LIB_SOURCES
    ${swap_table_cpp}
    )

set(LIB_HEADERS
    ${swap_table_hpp}
    psort.hpp
    ts_queue.hpp
    private/psort_private.hpp
)

set(TEST_SRC
    tests/RunAllTests.cpp
    tests/palgorithm.cpp
    )

register_library(${LIB_NAME} SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS} TEST_SOURCES ${TEST_SRC})

#test targets
addSourceFlags(psort.cpp COMPILE_FLAGS ${OpenMP_CXX_FLAGS})
turnOnAllWarnings(psort.cpp ${TEST_SRC})
add_executable(qsort_tester psort.cpp ${LIB_SOURCES})
addFlags(qsort_tester LINK_FLAGS ${OpenMP_CXX_FLAGS})
turnOnCpp11(qsort_tester)

#debug targets
add_executable(opt_test opt_test.cpp ${LIB_SOURCES})
turnOnCpp11(opt_test)

add_custom_target(dump ALL objdump -d -M intel opt_test > opt_test.txt DEPENDS opt_test)

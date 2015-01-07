
project( palgorithm )
cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED)
find_package(OpenMP)

add_subdirectory(bose-nelson)
generateswaptables(${CMAKE_CURRENT_BINARY_DIR} swap_tables swap_table_cpp swap_table_hpp)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

set(LIB_SOURCES
    ${swap_table_cpp}
    )

set(LIB_HEADERS
    ${swap_table_hpp}
    psort.hpp
    private/psort_private.hpp
)

set(TEST_SRC
    tests/RunAllTests.cpp
    tests/palgorithm.cpp
    )

add_library(palgorithm SHARED ${LIB_SOURCES} ${LIB_HEADERS})

#test targets
if(NOT WIN32)
	addSourceFlags(psort.cpp COMPILE_FLAGS ${OpenMP_CXX_FLAGS})
	add_executable(qsort_tester psort.cpp ${swap_table_cpp})
	addFlags(qsort_tester LINK_FLAGS ${OpenMP_CXX_FLAGS})

	#debug targets
	add_executable(opt_test opt_test.cpp ${swap_table_cpp})

	add_custom_target(dump ALL objdump -d -M intel opt_test > opt_test.txt DEPENDS opt_test)
endif(NOT WIN32)

register_target(NAME palgorithm SOURCES ${LIB_SOURCES} HEADERS ${LIB_HEADERS})

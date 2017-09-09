
find_package(Threads REQUIRED)
find_package(GMock REQUIRED)
find_package(GTest REQUIRED)

include_directories(SYSTEM ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src/putils)

set(SRC
        ts_resource_tests.cpp
    )

add_executable(putils_tests ${SRC})

target_link_libraries(putils_tests
                        PRIVATE
                            ${GMOCK_MAIN_LIBRARY}
                            ${GMOCK_LIBRARY}
                            ${GTEST_LIBRARY}
                            ${CMAKE_THREAD_LIBS_INIT}
)

enableCodeCoverage(putils_tests)

add_test(putils putils_tests)

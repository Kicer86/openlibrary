
find_package(Threads REQUIRED)
find_package(GMock REQUIRED)
find_package(GTest REQUIRED)

set(SRC
        configuration_tests.cpp
    )

add_executable(utils_qt_tests ${SRC})

target_link_libraries(utils_qt_tests
                        PRIVATE
                            ${GMOCK_MAIN_LIBRARY}
                            ${GMOCK_LIBRARY}
                            ${GTEST_LIBRARY}
                            ${CMAKE_THREAD_LIBS_INIT}
)

enableCodeCoverage(utils_qt_tests)

add_test(utils_qt utils_qt_tests)

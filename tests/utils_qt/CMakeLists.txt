
find_package(Threads REQUIRED)
find_package(GMock REQUIRED)
find_package(GTest REQUIRED)

set(SRC
        configuration_tests.cpp
        iconfiguration_storage_mock.hpp
    )

add_executable(utils_qt_tests ${SRC})

target_link_libraries(utils_qt_tests
                        PRIVATE
                            ${GMOCK_MAIN_LIBRARY}
                            ${GMOCK_LIBRARY}
                            ${GTEST_LIBRARY}
                            ${CMAKE_THREAD_LIBS_INIT}
                            utils_qt
)

target_include_directories(utils_qt_tests
                            PRIVATE
                                ${PROJECT_SOURCE_DIR}/src/includes
                                ${PROJECT_BINARY_DIR}/includes
                                ${GMOCK_INCLUDE_DIRS}
                                ${GTEST_INCLUDE_DIRS}
)

enableCodeCoverage(utils_qt_tests)

add_test(utils_qt utils_qt_tests)


find_package(Qt5Core REQUIRED)

add_library(utils_qt SHARED
                configuration.cpp configuration_p.hpp

                ${PROJECT_SOURCE_DIR}/src/includes/utils_qt/configuration.hpp
                ${PROJECT_SOURCE_DIR}/src/includes/utils_qt/iconfiguration.hpp
)

target_include_directories(utils_qt
                            PRIVATE
                                ${PROJECT_SOURCE_DIR}/src/includes/utils_qt
                                ${PROJECT_SOURCE_DIR}/src/includes
                                ${PROJECT_SOURCE_DIR}/src
                                ${PROJECT_BINARY_DIR}

                            SYSTEM PRIVATE
                                ${Qt5Core_INCLUDE_DIRS}
)

target_link_libraries(utils_qt
                        PRIVATE
                            Qt5::Core
)


register_target2(utils_qt)


find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(utils_qt SHARED
                configuration.cpp configuration_p.hpp
                configuration_json_storage.cpp

                ${PROJECT_SOURCE_DIR}/src/includes/OpenLibrary/utils_qt/configuration.hpp
                ${PROJECT_SOURCE_DIR}/src/includes/OpenLibrary/utils_qt/configuration_json_storage.hpp
                ${PROJECT_SOURCE_DIR}/src/includes/OpenLibrary/utils_qt/iconfiguration.hpp
)

target_include_directories(utils_qt
                            PRIVATE
                                ${PROJECT_SOURCE_DIR}/src               #todo: should be removed when all sublibs are setup as utils_qt
                                ${PROJECT_SOURCE_DIR}/src/includes
                                ${PROJECT_BINARY_DIR}/includes
)

target_link_libraries(utils_qt
                        PUBLIC
                            Qt::Core
)

register_target2(utils_qt)

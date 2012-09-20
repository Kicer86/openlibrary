project(router)

cmake_minimum_required( VERSION 2.6 )
find_package( Qt4 REQUIRED )

include_directories(${QT_INCLUDES} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)

set(routerDemo_SRCS
    main.cpp
    routerdemo.cpp
   )

set(router_HPP
    A_star/base.hpp
    A_star/std_alg.hpp
    A_star/std_types.hpp
)

add_executable( routerDemo ${routerDemo_SRCS})
target_link_libraries( routerDemo ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )

set_target_properties(routerDemo PROPERTIES COMPILE_FLAGS "--std=c++11"
                                            COMPILE_FLAGS_DEBUG "-Weffc++")

register_library(router, ${router_HPP})


project(router)

cmake_minimum_required( VERSION 2.6 )

set(routerDemo_SRCS
    main.cpp
   )

set(router_HPP
    A_star/base.hpp
    A_star/std_alg.hpp
    A_star/std_types.hpp
)

register_library(router HEADERS ${router_HPP})

include_directories(${QT_INCLUDES} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)
add_executable( routerDemo ${routerDemo_SRCS})

set_target_properties(routerDemo PROPERTIES COMPILE_FLAGS "--std=c++11"
                                            COMPILE_FLAGS_DEBUG "-Weffc++")

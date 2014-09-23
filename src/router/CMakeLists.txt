
project( router )

cmake_minimum_required( VERSION 2.6 )

include(${CMAKE_SOURCE_DIR}/cmake/functions.cmake)

set(routerDemo_SRCS
    main.cpp
   )

set(router_HPP
    A_star/base.hpp
    A_star/std_alg.hpp
    A_star/std_types.hpp
)

register_target(NAME router HEADERS ${router_HPP})

#visualizer
include_directories(${QT_INCLUDES} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)
add_executable(routerDemo ${routerDemo_SRCS})
turnOnCpp11(routerDemo)

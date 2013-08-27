
project( bose-nelson )
cmake_minimum_required(VERSION 2.8)

set(SRC
    bose-nelson.cpp
    )

set(HEADERS
    bose-nelson.hpp
)

add_executable(bose-nelson ${SRC})
turnOnCpp11(bose-nelson)

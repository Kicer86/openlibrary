
project( bose-nelson )
cmake_minimum_required(VERSION 2.8)

set(SRC 
    output_generator.cpp
    bose-nelson.cpp
    main.cpp
    )

set(HEADERS
    output_generator.hpp
    bose-nelson.hpp
)

add_executable(bose-nelson ${SRC})
turnOnCpp11(bose-nelson)

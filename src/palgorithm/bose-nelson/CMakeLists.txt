
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

function(generateSwapTables directory baseFileName cpp_file hpp_file)       #cpp_file and hpp_file will be returned
    
    set(_cpp_file ${directory}/${baseFileName}.cpp)
    set(_hpp_file ${directory}/${baseFileName}.hpp)
    
    set(${cpp_file} ${_cpp_file} PARENT_SCOPE)
    set(${hpp_file} ${_hpp_file} PARENT_SCOPE)
    
    set_source_files_properties(${_cpp_file} ${_hpp_file} PROPERTIES GENERATED TRUE)

    add_custom_command(OUTPUT ${_cpp_file} ${_hpp_file}
                       COMMAND bose-nelson ARGS ${_cpp_file} ${_hpp_file}
                       DEPENDS bose-nelson)
endfunction(generateSwapTables)


#some functions for generation useful stuff


#generate cmake variables for library
#Function generates file with library specific cmake variables
#A variable ${name}_LIBRARIES will be created.
#syntax: reguster_library_bin library_name bin1 bin2 bin3 ...
function(generate_variables name)

    #convert to uppercase
    string(TOUPPER ${name} UP_NAME)

    #generate file with cmake variables
    file(WRITE ${CMAKE_SOURCE_DIR}/scripts/tmp/${name} "set(${UP_NAME}_LIBRARIES ${ARGN})")

    set(GENERATED_FILES ${GENERATED_FILES} tmp/${name} PARENT_SCOPE)

endfunction(generate_variables)


#gather and connect all generated files with cmake variables
function(create_libraries_file)

    add_custom_command(OUTPUT libraries.cmake
                       COMMAND cat ARGS ${GENERATED_FILES} > libraries.cmake
                       DEPENDS ${GENERATED_FILES})

endfunction(create_libraries_file)

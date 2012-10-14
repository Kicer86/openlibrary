
#some functions for generation useful stuff

#generate cmake variables for library
#Function generates file with library specific cmake variables
#A variable ${name}_LIBRARIES will be created.
#syntax: reguster_library_bin library_name bin1 bin2 bin3 ...
function(generate_variables name)

    #convert to uppercase
    string(TOUPPER ${name} UP_NAME)

    #generate file with cmake variables
    add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/scripts/tmp/${name}
                       COMMAND echo ARGS "set(${UP_NAME}_LIBRARIES ${ARGN})" > ${CMAKE_SOURCE_DIR}/scripts/tmp/${name})

endfunction(generate_variables)


#gather and connect all generated files with cmake variables
function(generate_variables_file)

    file(GLOB_RECURSE files ${CMAKE_SOURCE_DIR}/scripts/tmp/*)
    foreach(file ${files})
        set(generated_files ${generated_files} ${file})
    endforeach(file ${files})

    message ( "Variables generated for libraries: ${generated_files}" )

    add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/scripts/libraries.cmake ALL
                      COMMAND cat ${generated_files} > ${CMAKE_SOURCE_DIR}/scripts/libraries.cmake
                      DEPENDS ${generated_files})

endfunction(generate_variables_file)

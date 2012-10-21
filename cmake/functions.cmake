
#some functions for generation useful stuff (internal use)

function(register_library name)

    set(var "")

    foreach(arg ${ARGN})

        if ("${arg}" MATCHES "(HEADERS)|(SOURCES)" )
            set(var ${arg})
        elseif (NOT "${var}" EQUAL "")
            set(${var}  ${${var}} ${arg})
        else()
            message(ERROR "unknown argument for register_library: ${arg}" )
        endif ("${arg}" MATCHES "(HEADERS)|(SOURCES)")

    endforeach(arg ${args})

    #c/c++ part
    if(SOURCES)
        set(LIBRARY_NAME ${OPENLIBRARY_SHORT_NAME}_${name})

        add_library(${LIBRARY_NAME} SHARED ${SOURCES})

        exportSymbols(${LIBRARY_NAME})

        #set_target_properties(${LIBRARY_NAME} PROPERTIES COMPILE_FLAGS "${LIB_CXXFLAGS}"
        #                                      COMPILE_FLAGS_DEBUG "-Weffc++")

        #install files
        if(WIN32)  # for windows (dll = runtime)
            set(RUNTIME_TYPE RUNTIME)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX})
        else(WIN32)
            set(RUNTIME_TYPE LIBRARY)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
        endif(WIN32)

        install (TARGETS ${LIBRARY_NAME} ${RUNTIME_TYPE} DESTINATION ${LIB_DESTINATION})

    endif(SOURCES)

    source_group(${LIBRARY_NAME}\\sources FILES ${SOURCES})
    source_group(${LIBRARY_NAME}\\headers FILES ${HEADERS})

    set(HEADERS_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/include/${OPENLIBRARY_DIR_NAME}/${name})

    install(FILES ${HEADERS}
            DESTINATION ${HEADERS_INSTALL_PREFIX}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    #some debug
    message("registering ${name} library")
    if(SOURCES)
        get_property(library_location TARGET ${LIBRARY_NAME} PROPERTY LOCATION)
        get_filename_component(library_file_name ${library_location} NAME)
        message("       Sources: ${SOURCES} -> ${library_file_name} -> installation path: ${LIB_DESTINATION}")

        #create variables for sub-library
        generate_variables(${name} ${LIBRARY_NAME})
    endif(SOURCES)

    if(HEADERS)
        message("       Headers: ${HEADERS} installatation path: ${HEADERS_INSTALL_PREFIX}")
    endif(HEADERS)

endfunction(register_library)


set(LIBRARIES_CMAKE ${PROJECT_SOURCE_DIR}/autogenerated/libraries.cmake)

#generate cmake variables for library
#Function generates file with library specific cmake variables
#A variable ${name}_LIBRARIES will be created.
#syntax: reguster_library_bin library_name bin1 bin2 bin3 ...
function(generate_variables name)

    #convert to uppercase
    string(TOUPPER ${name} UP_NAME)

    set(output ${PROJECT_SOURCE_DIR}/autogenerated/partial/${name})

    #generate file with cmake variables
    add_custom_command(OUTPUT ${output}
                       COMMAND echo ARGS 'set(${UP_NAME}_LIBRARIES ${ARGN})' > ${output}
                       DEPENDS ${PROJECT_SOURCE_DIR}/cmake/functions.cmake)   #depends on this file

    add_custom_target(${name}_variables DEPENDS ${output})

    add_dependencies(libraries.cmake ${name}_variables)

    set(OPENLIBRARY_REGISTERED_LIBRARIES
        ${output} ${OPENLIBRARY_REGISTERED_LIBRARIES}
        CACHE INTERNAL "List of registered sublibraries")

endfunction(generate_variables)


#gather and connect all generated files with cmake variables
function(generate_variables_file)

    set(dependencies ${OPENLIBRARY_REGISTERED_LIBRARIES})

    add_custom_command(OUTPUT ${LIBRARIES_CMAKE}
                       COMMAND cat ARGS "${PROJECT_SOURCE_DIR}/autogenerated/partial/*" > ${LIBRARIES_CMAKE}
                       DEPENDS ${dependencies}
                      )

    add_custom_target(libraries.cmake
                      DEPENDS ${LIBRARIES_CMAKE}
                      )

    #libraries.cmake is an autogenerated file, connect it to proper target
    add_dependencies(autogenerated libraries.cmake)

endfunction(generate_variables_file)


#function which creates file to by used by sublibraries to export theirs classes and functions

function(prepareExportFile filePath)

    file (WRITE ${filePath} "//OpenLibrary's automatically generated file. Do not edit.")

    #message(flag test "${MSVC}, ${MSVC_IDE}, ${MSVC60}, ${MSVC70}, ${MSVC71}, ${MSVC80}, ${CMAKE_COMPILER_2005}, ${MSVC90}, ${MSVC10} , ${CMAKE_GENERATOR}, ${WIN32}  ")

    if(CMAKE_COMPILER_IS_GNUCXX)
        file(APPEND ${filePath} "
            #ifndef OPENLIBRARY_INTERNAL_HPP
            #define OPENLIBRARY_INTERNAL_HPP

            #define OL_EXPORTS __attribute__ ((visibility (\"default\")))

            #endif
            ")
    else() #Visual Studio
        file(APPEND ${filePath} "
            #ifndef OPENLIBRARY_INTERNAL_HPP
            #define OPENLIBRARY_INTERNAL_HPP

            #ifdef OPENLIBRARY_DO_EXPORT
                #define OL_EXPORTS __declspec( dllexport )
            #else
                #define OL_EXPORTS __declspec( dllimport )
            #endif

            #endif
            ")
    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(prepareExportFile)

#functions providing platform independed compiler switches
function(turnOnCpp11 target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "--std=c++11")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnCpp11)


function(turnOnIntelSyntax target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-masm=intel")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnIntelSyntax)


function(exportSymbols target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
    else() #Visual Studio
        add_definitions(OPENLIBRARY_DO_EXPORT)
    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(exportSymbols)
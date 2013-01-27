
#some functions for generation useful stuff (internal use)

#register library
#a library target with given name will be created
function(register_library name)

    parseArguments(HEADERS SOURCES ARGUMENTS ${ARGN})

    #c/c++ part
    if(SOURCES)
		set(LIBRARY_NAME ${name})

        add_library(${LIBRARY_NAME} SHARED ${SOURCES})

        exportSymbols(${LIBRARY_NAME})
        turnOnAllWarnings(${LIBRARY_NAME})

        #install files
        if(WIN32)  # for windows (dll = runtime)
            set(RUNTIME_TYPE RUNTIME)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
        else(WIN32)
            set(RUNTIME_TYPE LIBRARY)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/OpenLibrary)
        endif(WIN32)

        install(TARGETS ${LIBRARY_NAME} ${RUNTIME_TYPE} DESTINATION ${LIB_DESTINATION})

    endif(SOURCES)

    source_group(${LIBRARY_NAME}\\sources FILES ${SOURCES})
    source_group(${LIBRARY_NAME}\\headers FILES ${HEADERS})

    getHeadersPath(HEADERS_INSTALL_PATH)
    set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${name})

    install(FILES ${HEADERS}
            DESTINATION ${HEADERS_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    #some debug
    message("registering ${name} library")
    if(SOURCES)
        get_property(library_location TARGET ${LIBRARY_NAME} PROPERTY LOCATION)
        get_filename_component(library_file_name ${library_location} NAME)
        message("       Sources: ${SOURCES} -> ${library_file_name} -> installation path: ${LIB_DESTINATION}")

        #create variables for sub-library
        list(GET ${HEADERS} 1 header)
        generate_variables(${name} ${LIBRARY_NAME} ${LIB_DESTINATION}/${library_file_name} ${HEADERS})
    endif(SOURCES)

    if(HEADERS)
        message("       Headers: ${HEADERS} installatation path: ${HEADERS_INSTALL_PATH}")
    endif(HEADERS)

endfunction(register_library)


#generate cmake variables for library
#Function generates file with library specific cmake variables
#A variable ${name}_LIBRARIES will be created.
#syntax: reguster_library_bin library_name bin1 bin2 bin3 ...
function(generate_variables name library libraryBinary)

    #as some unix commands are being used here - do not add anything on non unix systems
    if(UNIX AND NOT name STREQUAL "common")   #common is just the main OpenLibrary's package - don't create extra rules in FindOpenLibrary.cmake
        #convert to uppercase
        string(TOUPPER ${name} UP_NAME)

        set(output "${PROJECT_BINARY_DIR}/OpenLibrary_${name}.cmake")

        #add path to header files
        file(WRITE ${output} "set(OPENLIBRARY_${UP_NAME}_HEADERS ")
        foreach(header ${ARGN})
            file(APPEND ${output} "${OPENLIBRARY_DIR_NAME}/${library}/${header} ")
        endforeach(header ${ARGN})
        file(APPEND ${output} ")")

        #add path to library
        file(APPEND ${output} "\nset(OPENLIBRARY_${UP_NAME}_LIBRARIES ${libraryBinary})")

        install(FILES ${output} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenLibrary)

    endif(UNIX AND NOT name STREQUAL "common")

endfunction(generate_variables)


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

    #install file
    getHeadersPath(HEADERS_INSTALL_PATH)
    install(FILES ${filePath}
            DESTINATION ${HEADERS_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

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


function(turnOnAllWarnings target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-W -Wall -Wextra -Winit-self -Wformat=2 -Wshadow -Wlogical-op -Wsequence-point -Wfloat-equal -Weffc++ -Wold-style-cast")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnAllWarnings)


function(exportSymbols target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
    else() #Visual Studio
        add_definitions(/DOPENLIBRARY_DO_EXPORT)
    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(exportSymbols)


function(getHeadersPath path)
    set(${path} ${CMAKE_INSTALL_PREFIX}/include/${OPENLIBRARY_DIR_NAME} PARENT_SCOPE)
endfunction(getHeadersPath)


#usage:
#parseArguments( list of keywords_pattern 'ARGUMENTS' list of arguments to be parsed )
#function parses list of arguments to be parsed and sets a proper keyword variables.
#
#Example: parseArguments(AAA BBB ARGUMENTS AAA a b c d e BBB f g h i j CCC)
#will set variable AAA to "a b c d e"
#     and variable BBB to "f g h i j CCC"
function(parseArguments)

    set(arguments 0)
    set(keywords_pattern "")
    set(keywords "")
    set(var "")

    foreach(arg ${ARGN})

        if(arg STREQUAL "ARGUMENTS")
            set(arguments 1)
        else()

            if(arguments EQUAL 0)

                if(keywords_pattern STREQUAL "")
                    set(keywords_pattern "(${arg})")
                else()
                    set(keywords_pattern "${keywords_pattern}|(${arg})")
                endif(keywords_pattern STREQUAL "")

                set(keywords "${keywords};${arg}")

            else()  #arguments == 1

                if("${arg}" MATCHES "${keywords_pattern}" )
                    set(var ${arg})
                elseif (NOT "${var}" EQUAL "")
                    set(${var} "${${var}};${arg}")
                else()
                    message(ERROR "unknown argument: ${arg}" )
                endif ("${arg}" MATCHES "${keywords_pattern}")

            endif(arguments EQUAL 0)

        endif(arg STREQUAL "ARGUMENTS")

    endforeach()

    #export keywords
    foreach(keyword ${keywords})
        set(${keyword} ${${keyword}} PARENT_SCOPE)
    endforeach(keyword ${keywords})

endfunction(parseArguments)

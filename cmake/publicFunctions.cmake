
#some functions for generation useful stuff (public use)

#add flags to target
function(addFlags target propertyName flags)
    get_target_property(current_properties ${target} ${propertyName})

    if(NOT current_properties)
        set(current_properties "")
    endif(NOT current_properties)

    set_target_properties(${target} PROPERTIES ${propertyName} "${current_properties} ${flags}")
endfunction(addFlags)


#add flags to source file
function(addSourceFlags source propertyName flags)
    get_source_file_property(current_properties ${source} ${propertyName})

    if(NOT current_properties)
        set(current_properties "")
    endif(NOT current_properties)

    set_source_files_properties(${source} PROPERTIES ${propertyName} "${current_properties} ${flags}")
endfunction(addSourceFlags)

#functions providing platform independed compiler switches
function(turnOnCpp11 target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "--std=c++11")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnCpp11)


function(turnOnIntelSyntax target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "-masm=intel")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnIntelSyntax)


#add source files which should be checked for warnings
function(turnOnAllWarnings)

    foreach(source ${ARGN})

        get_source_file_property(isGenerated ${source} GENERATED)

        if (NOT isGenerated)

            if(CMAKE_COMPILER_IS_GNUCXX)
                addSourceFlags(${source} COMPILE_FLAGS "-W -Wall -Wextra -Winit-self -Wformat=2 -Wshadow -Wlogical-op -Wsequence-point -Wfloat-equal -Weffc++ -Wold-style-cast")
                #addSourceFlags(${source} COMPILE_FLAGS "-Werror")
            else() #Visual Studio

            endif(CMAKE_COMPILER_IS_GNUCXX)
        endif(NOT isGenerated)

    endforeach(source ${ARGN})

endfunction(turnOnAllWarnings)

function(enableCodeCoverage target)

    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "--coverage")
        addFlags(${target} LINK_FLAGS "--coverage")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(enableCodeCoverage)


function(hideSymbols target)

    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(hideSymbols)


function(addFirstIncludeDir sourceFile includeDir)

    if(CMAKE_COMPILER_IS_GNUCXX)
        addSourceFlags(${sourceFile} COMPILE_FLAGS "-isystem '${includeDir}'")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(addFirstIncludeDir)

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
                    set(${arg} "")                           #clean variable
                else()
                    set(keywords_pattern "${keywords_pattern}|(${arg})")
                endif(keywords_pattern STREQUAL "")

                set(keywords "${keywords};${arg}")

            else()  #arguments == 1

                if(arg MATCHES "${keywords_pattern}" )
                    set(var ${arg})
                elseif (NOT "${var}" EQUAL "")
                    set(${var} "${${var}};${arg}")
                else()
                    message(ERROR "unknown argument: ${arg}" )
                endif (arg MATCHES "${keywords_pattern}")

            endif(arguments EQUAL 0)

        endif(arg STREQUAL "ARGUMENTS")

    endforeach()

    #export keywords
    foreach(keyword ${keywords})
        set(${keyword} ${${keyword}} PARENT_SCOPE)
    endforeach(keyword ${keywords})

endfunction(parseArguments)


#function which creates file to by used by sublibraries to export theirs classes and functions
function(prepareExportFile filePath libraryName)

    file(WRITE ${filePath} "//OpenLibrary's automatically generated file. Do not edit.")
    string(TOUPPER ${libraryName} LIBRARY_NAME)

    #message(flag test "${MSVC}, ${MSVC_IDE}, ${MSVC60}, ${MSVC70}, ${MSVC71}, ${MSVC80}, ${CMAKE_COMPILER_2005}, ${MSVC90}, ${MSVC10} , ${CMAKE_GENERATOR}, ${WIN32}  ")

    if(CMAKE_COMPILER_IS_GNUCXX)
        file(APPEND ${filePath} "
            #ifdef ${libraryName}_EXPORTS
                #define ${LIBRARY_NAME}_EXPORTS __attribute__ ((visibility (\"default\")))
            #else
                #define ${LIBRARY_NAME}_EXPORTS
            #endif
            ")
    else() #Visual Studio
        file(APPEND ${filePath} "
            #ifdef ${libraryName}_EXPORTS
                #define ${LIBRARY_NAME}_EXPORTS __declspec( dllexport )
            #else
                #define ${LIBRARY_NAME}_EXPORTS __declspec( dllimport )
            #endif
            ")
    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(prepareExportFile)


function(getExportFile target filePath)

    set(${filePath} ${CMAKE_BINARY_DIR}/${target}_exports.hpp PARENT_SCOPE)  #create all export files ontop of binary dir to make them easy accessible

endfunction(getExportFile)


function(exportSymbols target)

    getExportFile(${target} generatedFile)
    prepareExportFile(${generatedFile} ${target})

endfunction(exportSymbols)

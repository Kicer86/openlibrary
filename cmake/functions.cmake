
#some functions for generation useful stuff (internal use)

set(OPENLIBRARY_DIR_NAME OpenLibrary)
set(TESTS_PERFORM_CODE_COVERAGE FALSE CACHE BOOL "If set to TRUE - additional flags are passed to all targets to allow code coverage. Also special make target is added")

#register library
#a library target with given name will be created
function(register_library name)

    parseArguments(HEADERS SOURCES ARGUMENTS ${ARGN})

    #c/c++ part
    if(SOURCES)
        set(LIBRARY_NAME ${name})

        add_library(${LIBRARY_NAME} SHARED ${SOURCES})

        exportSymbols(${LIBRARY_NAME})
        turnOnAllWarnings(${SOURCES})
        enableCodeCoverage(${LIBRARY_NAME})

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

    #convert to uppercase
    string(TOUPPER ${name} UP_NAME)

    set(output "${PROJECT_BINARY_DIR}/OpenLibrary_${name}Config.cmake")

    #add path to library
    file(WRITE  ${output} "\n")
    file(WRITE  ${output} "set(OPENLIBRARY_${UP_NAME}_LIBRARIES ${libraryBinary})\n")
    file(APPEND ${output} "set(OPENLIBRARY_LIBRARIES \${OPENLIBRARY_LIBRARIES} \${OPENLIBRARY_${UP_NAME}_LIBRARIES})\n")

    install(FILES ${output} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenLibrary)

endfunction(generate_variables)


#function which creates file to by used by sublibraries to export theirs classes and functions
function(prepareExportFile filePath)

    file (WRITE ${filePath} "//OpenLibrary's automatically generated file. Do not edit.")

    #message(flag test "${MSVC}, ${MSVC_IDE}, ${MSVC60}, ${MSVC70}, ${MSVC71}, ${MSVC80}, ${CMAKE_COMPILER_2005}, ${MSVC90}, ${MSVC10} , ${CMAKE_GENERATOR}, ${WIN32}  ")

    if(CMAKE_COMPILER_IS_GNUCXX)
        file(APPEND ${filePath} "
            #ifndef OPENLIBRARY_INTERNAL_HPP
            #define OPENLIBRARY_INTERNAL_HPP

            #ifdef OPENLIBRARY_DO_EXPORT
                #define OL_EXPORTS __attribute__ ((visibility (\"default\")))
            #else
                #define OL_EXPORTS
            #endif

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

function(addFlags target propertyName flags)
    get_target_property(current_properties ${target} ${propertyName})

    if(NOT current_properties)
        set(current_properties "")
    endif(NOT current_properties)

    set_target_properties(${target} PROPERTIES ${propertyName} "${current_properties} ${flags}")
endfunction(addFlags)


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
                addSourceFlags(${source} COMPILE_FLAGS "-Werror")
            else() #Visual Studio

            endif(CMAKE_COMPILER_IS_GNUCXX)
        endif(NOT isGenerated)

    endforeach(source ${ARGN})

endfunction(turnOnAllWarnings)

function(enableCodeCoverage target)

    if(TESTS_PERFORM_CODE_COVERAGE)
        addFlags(${target} COMPILE_FLAGS "--coverage")
        addFlags(${target} LINK_FLAGS "--coverage")
    endif(TESTS_PERFORM_CODE_COVERAGE)

endfunction(enableCodeCoverage)

##### end of usefull switches #####

function(exportSymbols target)

    add_definitions(-DOPENLIBRARY_DO_EXPORT)

    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(exportSymbols)


function(getHeadersBasePath path)
    set(${path} ${CMAKE_INSTALL_PREFIX}/include/ PARENT_SCOPE)
endfunction(getHeadersBasePath)

function(getHeadersPath path)
    getHeadersBasePath(basePath)
    set(${path} ${basePath}/${OPENLIBRARY_DIR_NAME} PARENT_SCOPE)
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


#register test program. provide library name and sources as parameters
function(registerTest libraryName)

    find_package(CppUTest)

    if(CPPUTEST_FOUND)

        set(sources ${ARGN})
        set(targetName ${libraryName}Tests)

        message("${libraryName}: adding tests - CppUTest package found")
        include_directories(${CPPUTEST_INCLUDE_DIRS})
        add_executable(${targetName} ${sources})

        target_link_libraries(${targetName} ${CPPUTEST_LIBRARIES})

        add_custom_target(perform_${targetName}
                          COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${targetName}
                          DEPENDS ${targetName})

        turnOnCpp11(${targetName})
        enableCodeCoverage(${targetName})

        #attach perform${targetName} to 'test' target
        add_dependencies(test perform_${targetName})

        #extra tools
        if(UNIX)

            #create valgrind tests
            find_program(valgrindPath valgrind)

            if(valgrindPath)

                add_custom_target(valgrind_${targetName}
                                  COMMAND ${valgrindPath} --error-exitcode=255 ${CMAKE_CURRENT_BINARY_DIR}/${targetName} &> /dev/null\; if [ $$? -eq 255 ]\; then echo "valgrind problems when running ${CMAKE_CURRENT_BINARY_DIR}/${targetName}" \; exit 1\; else echo "running valgrind for ${targetName}: OK" \; exit 0\; fi
                                  DEPENDS ${targetName}
                                 )
                add_dependencies(valgrind_test valgrind_${targetName})

            endif(valgrindPath)

            if(TESTS_PERFORM_CODE_COVERAGE)

                get_target_property(projSources ${libraryName} SOURCES)
                get_target_property(libraryFile ${libraryName} LOCATION)

                #run code coverage tool
                add_custom_target(gcov_${targetName}
                                  COMMAND sh ${CMAKE_SOURCE_DIR}/code_cov.sh
                                          ${CMAKE_CURRENT_BINARY_DIR}/${targetName}
                                          ${libraryFile}
                                          ${projSources} ${sources}
                                 )

                add_dependencies(test_code_coverage gcov_${targetName})

            endif(TESTS_PERFORM_CODE_COVERAGE)

        endif(UNIX)

    endif(CPPUTEST_FOUND)

endfunction(registerTest)

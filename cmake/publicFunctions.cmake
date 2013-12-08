
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
    elseif(MSVC) #Visual Studio
    
        #on by default
        
    else()
    
        #assumption it's llvm
        addFlags(${target} COMPILE_FLAGS "-std=c++11")

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnCpp11)


function(turnOnIntelSyntax target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "-masm=intel")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction(turnOnIntelSyntax)


function(enableCodeCoverageForSources target) #after target go sources

    #sources
    foreach(source ${ARGN})
       
        if(CMAKE_COMPILER_IS_GNUCXX)
            addSourceFlags(${source} COMPILE_FLAGS "--coverage")

        elseif(MSVC) #Visual Studio
                    
        else() 
        
            #assumption it's llvm
            addSourceFlags(${source} COMPILE_FLAGS "--coverage")
            
        endif(CMAKE_COMPILER_IS_GNUCXX)
        
    endforeach(source ${ARGN})

    #linker
    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} LINK_FLAGS "--coverage")

    elseif(MSVC) #Visual Studio
                        
    else() 
        
        #assumption it's llvm
        addFlags(${target} LINK_FLAGS "--coverage")

    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(enableCodeCoverageForSources)


# function registers target which uses 'lcov' tool to generate html with code coverage
#
# as arguments use target to be coveraged
function(enableCodeCoverage target)
    
    #TODO: remove cov database files before running tests (gcda gcno) as they may be not up to date with sources
   
    #TODO: some errors
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
   
    #register lcov's global targets
    if (LCOV AND NOT TARGET lcov_generate)   #register lcov targets only once, for whole binary tree

        #constants with paths
        set(HTML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/code_coverage/html)
               
        #init counters
        add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/lcov/clear 
                           COMMAND lcov --directory ${CMAKE_BINARY_DIR} --zerocounters
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lcov
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/code_coverage
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${HTML_OUTPUT_DIR}
                           COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/lcov/*
                           COMMAND ${CMAKE_COMMAND} -E remove -f ${HTML_OUTPUT_DIR}/index.html  #to force genhtml
                           COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/lcov/clear
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                           COMMENT "cleaning code coverage data")
                
        #gather info and generate html
        add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/lcov/test_run
                           DEPENDS ${CMAKE_BINARY_DIR}/lcov/clear
                           DEPENDS ${target}
                           COMMAND ctest
                           COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/lcov/test_run
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                           COMMENT "running tests")
                           
        add_custom_target(_lcov_prepare
                          DEPENDS ${CMAKE_BINARY_DIR}/lcov/test_run
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                          COMMENT "preparing lcov environment")
                          
        add_custom_target(_lcov_gather_data                                #target-related targets attach here
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                          COMMENT "gathering code coverage data")
                                   
        add_custom_command(OUTPUT ${HTML_OUTPUT_DIR}/index.html
                           DEPENDS _lcov_gather_data
                           COMMAND ${GENHTML} --quiet ${CMAKE_BINARY_DIR}/lcov/lcov_*.info --output-directory ${HTML_OUTPUT_DIR}
                           COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/lcov/clear #not clean anymore
                           COMMENT "generating html with code coverage information"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
                           
        add_custom_target(lcov_generate
                          DEPENDS ${HTML_OUTPUT_DIR}/index.html
                          COMMENT "generating lcov data"
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
                                                     
    endif(LCOV AND NOT TARGET lcov_generate)
    
    #per target build step
    if(LCOV)
        get_property(LIB_LOCATION TARGET ${target} PROPERTY LOCATION)
        get_filename_component(LIB_DIR ${LIB_LOCATION} PATH)
        add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/lcov/lcov_${target}.info
                           DEPENDS _lcov_prepare
                           COMMAND ${LCOV} --quiet --capture --directory . --output-file ${CMAKE_BINARY_DIR}/lcov/lcov_${target}.info
                           COMMAND ${LCOV} --quiet --remove ${CMAKE_BINARY_DIR}/lcov/lcov_${target}.info '/usr/include/*' '/usr/lib/*' -o ${CMAKE_BINARY_DIR}/lcov/lcov_${target}.info
                           WORKING_DIRECTORY ${LIB_DIR}
                           COMMENT "gathering code coverage data for target ${target}")
                           
        add_custom_target(_lcov_${target}_info_gathering
                          DEPENDS ${CMAKE_BINARY_DIR}/lcov/lcov_${target}.info
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

        add_dependencies(_lcov_gather_data _lcov_${target}_info_gathering)
                          
        get_property(sources TARGET ${target} PROPERTY SOURCES)
        enableCodeCoverageForSources(${target} ${sources})
    endif(LCOV)
    
endfunction(enableCodeCoverage)


#Enables gtest for target. 'target' will be linked with GTest's or GMock's main library depending on mode (use value GTEST or GMOCK)
function(enableGTest target mode)
        
    if("${mode}" STREQUAL "GTEST")
        set(link_library ${GTEST_MAIN_LIBRARY} ${GTEST_LIBRARY})  #in GTest 1.7 main library contains only mian function. Linking against base library is required
    elseif("${mode}" STREQUAL "GMOCK")
        set(link_library ${GMOCK_MAIN_LIBRARY})
    else()
        message(FATAL_ERROR "For 'mode' argument use 'GTEST' or 'GMOCK'. Currently ${mode} was provided")
    endif()
    
    #prepare test target
    find_package (Threads)
    exec_program(${CMAKE_COMMAND} ARGS -E touch ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
    add_executable(${target}_test ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
    target_link_libraries(${target}_test 
                          ${target}                     #library with code and tests
                          ${link_library}               #gmock or gtest library
                          ${CMAKE_THREAD_LIBS_INIT}     #pthreads
                         )
    
    add_test(${target} ${target}_test)

endfunction(enableGTest)


function(hideSymbols target)

    if(CMAKE_COMPILER_IS_GNUCXX)
        addFlags(${target} COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
    else() #Visual Studio

    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(hideSymbols)


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

    get_target_property(targetType ${libraryName} TYPE)

    if(targetType STREQUAL "SHARED_LIBRARY")

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

    else()
        file(APPEND ${filePath} "\n#define ${LIBRARY_NAME}_EXPORTS\n")
    endif(targetType STREQUAL "SHARED_LIBRARY")

endfunction(prepareExportFile)


function(getExportFile target filePath)

    set(${filePath} ${CMAKE_BINARY_DIR}/${target}_exports.hpp PARENT_SCOPE)  #create all export files ontop of binary dir to make them easy accessible

endfunction(getExportFile)


function(exportSymbols target)

    getExportFile(${target} generatedFile)
    prepareExportFile(${generatedFile} ${target})

endfunction(exportSymbols)

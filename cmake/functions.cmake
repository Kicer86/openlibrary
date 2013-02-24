
#some functions for generation useful stuff (internal use)

set(OPENLIBRARY_DIR_NAME OpenLibrary)
include(${CMAKE_SOURCE_DIR}/cmake/publicFunctions.cmake)

#register library
#a library target with given name will be created
function(register_library name)

    parseArguments(HEADERS SOURCES TEST_SOURCES ARGUMENTS ${ARGN})
    #SOURCES: list of all cpp files
    #HEADERS: list of headers which are meant to be published
    #TEST_SOURCES: source files of unit tests

    set(LIBRARY_NAME ${name})

    #c/c++ part
    if(SOURCES)

        add_library(${LIBRARY_NAME} SHARED ${SOURCES})

        exportSymbols(${LIBRARY_NAME})
        turnOnAllWarnings(${SOURCES})

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
    set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${LIBRARY_NAME})

    install(FILES ${HEADERS}
            DESTINATION ${HEADERS_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    #some debug
    message("registering ${LIBRARY_NAME} library")
    if(SOURCES)
        get_property(library_location TARGET ${LIBRARY_NAME} PROPERTY LOCATION)
        get_filename_component(library_file_name ${library_location} NAME)
        message("       Sources: ${SOURCES} -> ${library_file_name} -> installation path: ${LIB_DESTINATION}")

        #create variables for sub-library
        list(GET ${HEADERS} 1 header)
        generate_cmakeConfig(${LIBRARY_NAME} ${LIB_DESTINATION}/${library_file_name} ${HEADERS})
    endif(SOURCES)

    if(HEADERS)
        message("       Headers: ${HEADERS} installatation path: ${HEADERS_INSTALL_PATH}")
    endif(HEADERS)

    #test
    if(TEST_SOURCES)
        registerTest(${LIBRARY_NAME} ${SOURCES} ${TEST_SOURCES})
    endif(TEST_SOURCES)

    #generate export rules
    generateExportFile(${LIBRARY_NAME})

endfunction(register_library)


#generate cmake variables for library
#Function generates file with library specific cmake variables
#A variable ${library}_LIBRARIES will be created.
#syntax: reguster_library_bin library_name bin1 bin2 bin3 ...
function(generate_cmakeConfig library libraryBinary)

    #convert to uppercase
    string(TOUPPER ${library} UP_NAME)

    set(output "${PROJECT_BINARY_DIR}/OpenLibrary_${library}Config.cmake")

    #add path to library
    file(WRITE  ${output} "\n")
    file(WRITE  ${output} "set(OPENLIBRARY_${UP_NAME}_LIBRARIES ${libraryBinary})\n")
    file(APPEND ${output} "set(OPENLIBRARY_LIBRARIES \${OPENLIBRARY_LIBRARIES} \${OPENLIBRARY_${UP_NAME}_LIBRARIES})\n")

    install(FILES ${output} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenLibrary)

endfunction(generate_cmakeConfig)


#function which creates file to by used by sublibraries to export theirs classes and functions
function(prepareExportFile filePath libraryName)

    file (WRITE ${filePath} "//OpenLibrary's automatically generated file. Do not edit.")
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
            #ifdef ${libraryName}
                #define ${LIBRARY_NAME}_EXPORTS __declspec( dllexport )
            #else
                #define ${LIBRARY_NAME}_EXPORTS __declspec( dllimport )
            #endif
            ")
    endif(CMAKE_COMPILER_IS_GNUCXX)

endfunction(prepareExportFile)


function(generateExportFile libraryName)

    set(generatedFile ${CMAKE_CURRENT_BINARY_DIR}/${libraryName}_exports.hpp)
    prepareExportFile(${generatedFile} ${libraryName})

    getHeadersPath(HEADERS_INSTALL_PATH)
    set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${LIBRARY_NAME})

    install(FILES ${generatedFile}
            DESTINATION ${HEADERS_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

endfunction(generateExportFile libraryName)


function(getHeadersBasePath path)
    set(${path} ${CMAKE_INSTALL_PREFIX}/include/ PARENT_SCOPE)
endfunction(getHeadersBasePath)

function(getHeadersPath path)
    getHeadersBasePath(basePath)
    set(${path} ${basePath}/${OPENLIBRARY_DIR_NAME} PARENT_SCOPE)
endfunction(getHeadersPath)


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

            #code coverage
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


        endif(UNIX)

    endif(CPPUTEST_FOUND)

endfunction(registerTest)

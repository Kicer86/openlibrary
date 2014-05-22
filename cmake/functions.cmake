
#some functions for generation useful stuff (internal use)

set(OPENLIBRARY_DIR_NAME OpenLibrary)
include(${CMAKE_SOURCE_DIR}/cmake/publicFunctions.cmake)

#register library
#a library target with given name will be created
function(register_library name)

    parseArguments(HEADERS SOURCES LIBRARIES TEST_SOURCES ARGUMENTS ${ARGN})

    #SOURCES: list of all cpp files
    #HEADERS: list of headers which are meant to be published
    #LIBRARIES: list of libraries to be linked
    #TEST_SOURCES: source files of unit tests

    set(LIBRARY_NAME ${name})

    #c/c++ part
    if(SOURCES)

        add_library(${LIBRARY_NAME} SHARED ${SOURCES})
        target_link_libraries(${LIBRARY_NAME} ${LIBRARIES})

        hideSymbols(${LIBRARY_NAME})

        #install files
        if(WIN32)  # for windows (dll = runtime)
            set(RUNTIME_TYPE RUNTIME)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
        else(WIN32)
            set(RUNTIME_TYPE LIBRARY)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/OpenLibrary)
        endif(WIN32)

        install(TARGETS ${LIBRARY_NAME}
                EXPORT OpenLibrary_${LIBRARY_NAME}Config
                DESTINATION ${LIB_DESTINATION})

        install(EXPORT OpenLibrary_${LIBRARY_NAME}Config DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenLibrary/private)

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

        get_property(library_location TARGET ${LIBRARY_NAME} PROPERTY LOCATION)              #get library file
        get_filename_component(library_file_name ${library_location} NAME)

        #create variables for sub-library
        list(GET ${HEADERS} 1 header)
    endif(SOURCES)

    #test
    if(TEST_SOURCES)
        registerTest(${LIBRARY_NAME} SOURCES ${TEST_SOURCES} ${SOURCES} LIBRARIES ${LIBRARIES})
    endif(TEST_SOURCES)

    #generate export rules
    if(SOURCES)
        set(header ${CMAKE_BINARY_DIR}/${LIBRARY_NAME}_export.h)
        generate_export_header(${LIBRARY_NAME} EXPORT_FILE_NAME ${header})

        getHeadersPath(HEADERS_INSTALL_PATH)
        set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${libraryName})

        install(FILES ${header}
                DESTINATION ${HEADERS_INSTALL_PATH}
                PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
    endif(SOURCES)

endfunction(register_library)


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

        unset(SOURCES)
        unset(LIBRARIES)

        #TODO: use CMakeParseArguments
        parseArguments(SOURCES LIBRARIES ARGUMENTS ${ARGN})

        set(targetName ${libraryName}Tests)

        message("${libraryName}: adding tests - CppUTest package found")
        include_directories(${CPPUTEST_INCLUDE_DIRS})

        add_executable(${targetName} ${SOURCES})                           #create extra test target binary with proper sources
        target_link_libraries(${targetName} ${CPPUTEST_LIBRARIES})

        add_test(${targetName}_tests ${targetName})
        target_link_libraries(${targetName} ${LIBRARIES})

        turnOnCpp11(${targetName})
        enableCodeCoverage(${targetName})

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

        endif(UNIX)

    endif(CPPUTEST_FOUND)

endfunction(registerTest)


function(add_library_path path name)

    set(enable TRUE)
    if("${ARGN}" STREQUAL "EXPERIMENTAL")
        set(enable FALSE)
    endif("${ARGN}" STREQUAL "EXPERIMENTAL")

    string(TOUPPER ${name} NAME)
    set(LIB_VARIABLE OPENLIBRARY_${NAME}_BUILD)
    set(${LIB_VARIABLE} ${enable} CACHE BOOL "Build library ${name}")

    if(${LIB_VARIABLE})
        add_subdirectory(${path})
    endif(${LIB_VARIABLE})

endfunction(add_library_path)

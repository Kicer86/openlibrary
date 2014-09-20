
#some functions for generation useful stuff (internal use)

set(OPENLIBRARY_DIR_NAME OpenLibrary)
include(${CMAKE_SOURCE_DIR}/cmake/OpenLibraryUse.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/private_functions.cmake)

function(register_target)

    set(options)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES HEADERS)
    cmake_parse_arguments(rt "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    register_target_install_lib(${rt_NAME})
    register_target_install_headers(${rt_NAME} ${rt_HEADERS})
    register_target_set_groups(SOURCES ${rt_SOURCES} HEADERS ${rt_HEADERS})
    register_target_set_version(${rt_NAME})
    register_target_export_header(${rt_NAME} ${rt_SOURCES})

    hideSymbols(${rt_NAME})

endfunction()


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

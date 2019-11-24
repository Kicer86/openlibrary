
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


function(register_target2 target_name)

    get_target_property(source_files ${target_name} SOURCES)

    register_target_install_lib(${target_name})
    register_target_install_headers2(${target_name})
    register_target_set_groups2(${source_files})
    register_target_set_version(${target_name})
    register_target_export_header2(${target_name})

    hideSymbols(${target_name})

endfunction()


function(getHeadersBasePath path)
    set(${path} ${CMAKE_INSTALL_PREFIX}/include/ PARENT_SCOPE)
endfunction(getHeadersBasePath)


function(getHeadersPath path)
    getHeadersBasePath(basePath)
    set(${path} ${basePath}/${OPENLIBRARY_DIR_NAME} PARENT_SCOPE)
endfunction(getHeadersPath)


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

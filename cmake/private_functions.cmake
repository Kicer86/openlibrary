
#private functions used by register_target()
function(register_target_install_lib name)

    set(LIBRARY_NAME ${name})

    #install files
    if(WIN32)
        set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
    else(WIN32)
        set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/OpenLibrary)
    endif(WIN32)

    if(TARGET ${LIBRARY_NAME})     #target may not exist if there are no sources (just headers)

        install(TARGETS ${LIBRARY_NAME}
                EXPORT OpenLibrary_${LIBRARY_NAME}Config
                DESTINATION ${LIB_DESTINATION})

        install(EXPORT OpenLibrary_${LIBRARY_NAME}Config
                DESTINATION ${CMAKE_INSTALL_PREFIX}/${DEF_INSTALL_CMAKE_DIR}/private
                NAMESPACE ${OPENLIBRARY_NAMESPACE})

     else()

        configure_file(${CMAKE_SOURCE_DIR}/cmake/OpenLibrary_HeadersLibConfig.cmake.in ${CMAKE_BINARY_DIR}/OpenLibrary_${LIBRARY_NAME}Config.cmake @ONLY)
        install(FILES ${CMAKE_BINARY_DIR}/OpenLibrary_${LIBRARY_NAME}Config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/${DEF_INSTALL_CMAKE_DIR}/private)

    endif(TARGET ${LIBRARY_NAME})

endfunction(register_target_install_lib)


function(register_target_set_groups)

    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES HEADERS)
    cmake_parse_arguments(rtsg_ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    source_group(${LIBRARY_NAME}\\sources FILES ${rtsg_SOURCES})
    source_group(${LIBRARY_NAME}\\headers FILES ${rtsg_HEADERS})

endfunction(register_target_set_groups)


function(register_target_set_groups2 files)

    source_group(${LIBRARY_NAME}\\sources FILES ${files})

endfunction(register_target_set_groups2)


function(register_target_install_headers name)

    set(LIBRARY_NAME ${name})
    set(HEADERS ${ARGN})

    getHeadersPath(HEADERS_INSTALL_PATH)
    set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${LIBRARY_NAME})

    install(FILES ${HEADERS}
            DESTINATION ${HEADERS_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    #some debug
    message("registering ${LIBRARY_NAME} library")

endfunction(register_target_install_headers)


function(register_target_install_headers2 name)

    getHeadersPath(HEADERS_INSTALL_PATH)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/includes/OpenLibrary/${name}
            DESTINATION ${HEADERS_INSTALL_PATH}
            FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )

    #some debug
    message("registering ${LIBRARY_NAME} library")

endfunction(register_target_install_headers2)


function(register_target_set_version target)

    if(TARGET ${target})

        set_target_properties(${target} PROPERTIES
                            VERSION ${OPENLIBRARY_VERSION}
                            SOVERSION ${OPENLIBRARY_MAJOR_VERSION})

    endif(TARGET ${target})

endfunction(register_target_set_version)


#target and sources as arguments
function(register_target_export_header target)

    if(ARGN AND TARGET ${target})
        set(header ${CMAKE_BINARY_DIR}/${target}_export.h)
        generate_export_header(${target} EXPORT_FILE_NAME ${header})

        getHeadersPath(HEADERS_INSTALL_PATH)
        set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${target})

        install(FILES ${header}
                DESTINATION ${HEADERS_INSTALL_PATH}
                PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
    endif()

endfunction(register_target_export_header)


function(register_target_export_header2 target)

    if(TARGET ${target})
        set(header ${CMAKE_BINARY_DIR}/includes/OpenLibrary/${target}/${target}_export.h)
        generate_export_header(${target} EXPORT_FILE_NAME ${header})

        getHeadersPath(HEADERS_INSTALL_PATH)
        set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${target})

        install(FILES ${header}
                DESTINATION ${HEADERS_INSTALL_PATH}
                PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
    endif()

endfunction(register_target_export_header2)

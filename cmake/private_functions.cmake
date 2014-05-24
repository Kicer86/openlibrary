
#private funcitons used by register_target()
function(register_target_add_install name)

    set(LIBRARY_NAME ${name})

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

    install(EXPORT OpenLibrary_${LIBRARY_NAME}Config DESTINATION ${CMAKE_INSTALL_PREFIX}/${DEF_INSTALL_CMAKE_DIR}/private)

endfunction(register_target_add_install)


function(register_target_set_groups)

    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES HEADERS)
    cmake_parse_arguments(rtsg_ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    source_group(${LIBRARY_NAME}\\sources FILES ${rtsg_SOURCES})
    source_group(${LIBRARY_NAME}\\headers FILES ${rtsg_HEADERS})

endfunction(register_target_set_groups)


function(register_target_install_headers)

    getHeadersPath(HEADERS_INSTALL_PATH)
    set(HEADERS_INSTALL_PATH ${HEADERS_INSTALL_PATH}/${LIBRARY_NAME})

    install(FILES ${HEADERS}
            DESTINATION ${HEADERS_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    #some debug
    message("registering ${LIBRARY_NAME} library")

endfunction(register_target_install_headers)


function(register_target_set_version target)

    set_target_properties(${target} PROPERTIES
                          VERSION ${OPENLIBRARY_VERSION}
                          SOVERSION ${OPENLIBRARY_MAJOR_VERSION})

endfunction(register_target_set_version)

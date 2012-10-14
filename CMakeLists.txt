
project(library)
cmake_minimum_required(VERSION 2.8)

include( ${CMAKE_SOURCE_DIR}/OpenLibrary.cmake )

set(LIB_CXXFLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

#some usefull fuctions
function(register_library name)

    set(var "")

    foreach(arg ${ARGN})

        if ("${arg}" MATCHES "(HEADERS)|(SOURCES)" )
            set(var ${arg})
        elseif (NOT "${var}" EQUAL "")
            set(${var}  ${${var}} ${arg})
        else()
            message(ERROR "unknown argument for register_library: ${arg}" )
        endif ("${arg}" MATCHES "(HEADERS)|(SOURCES)")

    endforeach(arg ${args})

    if(SOURCES)
        set(LIBRARY_NAME ${OPENLIBRARY_SHORT_NAME}_${name})

        add_library(${LIBRARY_NAME} SHARED ${SOURCES})
        set_target_properties(${LIBRARY_NAME} PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS} -masm=intel"
                                              COMPILE_FLAGS_DEBUG "-Weffc++")

        #install files
        if(WIN32)  # for windows (dll = runtime)
            set(RUNTIME_TYPE RUNTIME)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX})
        else(WIN32)
            set(RUNTIME_TYPE LIBRARY)
            set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
        endif(WIN32)

        install (TARGETS ${LIBRARY_NAME} ${RUNTIME_TYPE} DESTINATION ${LIB_DESTINATION})

    endif(SOURCES)

    set(HEADERS_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/include/${OPENLIBRARY_DIR_NAME}/${name})

    install (FILES ${HEADERS}
             DESTINATION ${HEADERS_INSTALL_PREFIX}
             PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    #sime debug
    message("library ${name} registered.")
    if(SOURCES)
        message("       Sources: ${SOURCES} -> installation path: ${LIB_DESTINATION}")
    endif(SOURCES)

    if(HEADERS)
        message("       Headers: ${HEADERS} installatation path: ${HEADERS_INSTALL_PREFIX}")
    endif(HEADERS)

endfunction(register_library)


add_subdirectory(htmlParser htmlParser)
#add_subdirectory(qextserialport qextserialport)
add_subdirectory(router router)
add_subdirectory(QtExt QtExt)
add_subdirectory(utils utils)
add_subdirectory(math math)

set(LIBRARY_HEADERS debug.hpp std_macros.hpp str_op.hpp)

register_library(common HEADERS ${LIBRARY_HEADERS})

# install(FILES cmake/ForceAddFlags.cmake
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/Modules
#         PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

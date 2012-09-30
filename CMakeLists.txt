
project(library)
cmake_minimum_required(VERSION 2.8)


set(LIB_CXXFLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
set(OPEN_LIBRARY_DIR OpenLib)

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

    if (NOT ${SOURCES} EQUAL "")
        add_library(${name} SHARED ${SOURCE} )

        set_target_properties(${name} PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS} -masm=intel"
                                                 COMPILE_FLAGS_DEBUG "-Weffc++")

        #install files
        if(WIN32)  # for windows (dll = runtime)
            install (TARGETS ${name} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
            set(LIB_INSTALLATION_DIR ${CMAKE_INSTALL_PREFIX})
        else(WIN32)
            install (TARGETS ${name} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
            set(LIB_INSTALLATION_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
        endif(WIN32)

    endif (NOT ${SOURCES} EQUAL "")

    set(HEADERS_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/include/${OPEN_LIBRARY_DIR}/${name})
    install (FILES ${HEADERS}
             DESTINATION ${HEADERS_INSTALL_PREFIX}
             PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    message("library ${name} registered.")
    if(SOURCES)
        message("       Sources: ${SOURCES} -> installation path: ${LIB_INSTALLATION_DIR}")
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

set(LIBRARY_HEADERS debug.hpp std_macros.hpp str_op.hpp)

register_library(common HEADERS ${LIBRARY_HEADERS})

# install(FILES cmake/ForceAddFlags.cmake
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/Modules
#         PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

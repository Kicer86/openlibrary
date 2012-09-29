
project(library)
cmake_minimum_required(VERSION 2.8)


set(LIB_CXXFLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

#some usefull fuctions
function(register_library args)

    set(name "")
    set(var "")

    foreach(arg ${args})

        if ("${name}" EQUAL "")
            set(name ${arg})
        elseif ("${arg}" MATCHES "(HEADERS)|(SOURCES)" )
            set(var ${arg})
        elseif (NOT "${var}" EQUAL "")
            set(${${var}} ${arg})
        else()
            message(ERROR "unknown argument for register_library: ${arg}" )
        endif ("${name}" EQUAL "")

    endforeach(arg ${args})

    if (NOT ${sources} EQUAL "")
        add_library(${name} SHARED ${sources} )

        set_target_properties(${name} PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS} -masm=intel"
                                                 COMPILE_FLAGS_DEBUG "-Weffc++")

        #install files
        if(WIN32)  # for windows (dll = runtime)
            install (TARGETS ${name} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
        else(WIN32)
            install (TARGETS ${name} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
        endif(WIN32)

    endif (NOT ${sources} EQUAL "")

    install (FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

endfunction(register_library)


add_subdirectory(htmlParser htmlParser)
#add_subdirectory(qextserialport qextserialport)
add_subdirectory(router router)
add_subdirectory(QtExt QtExt)
add_subdirectory(utils utils)

set(LIBRARY_HEADERS debug.hpp std_macros.hpp str_op.hpp)

install(FILES ${LIBRARY_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ )

# install(FILES cmake/ForceAddFlags.cmake
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/Modules
#         PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

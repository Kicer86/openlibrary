project(library)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(htmlParser htmlParser)
add_subdirectory(qextserialport qextserialport)
add_subdirectory(router router)
add_subdirectory(QtExt QtExt)

set(LIBRARY_HEADERS debug.hpp std_macros.hpp str_op.hpp)

install(FILES ${LIBRARY_HEADERS} 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include 
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ )
        
# install(FILES cmake/ForceAddFlags.cmake 
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/Modules
#         PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

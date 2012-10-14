set(LIB_NAME utils)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

set(LIB_SOURCES
    cpuinfo.cpp
    )

set(LIB_HEADERS
    copy_ptr.hpp
    cpuinfo.hpp
)

add_library(${LIB_NAME} SHARED ${LIB_SOURCES} )


set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS} -masm=intel"
                                  COMPILE_FLAGS_DEBUG "-Weffc++")


#install files
if(WIN32)  # for windows (dll = runtime)
  install(TARGETS ${LIB_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else(WIN32)
  install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif(WIN32)

install(FILES ${LIB_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

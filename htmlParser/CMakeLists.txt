project( htmlParser )
cmake_minimum_required(VERSION 2.8)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DYYDEBUG=1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(HTMLPARSER_SRC
  searchlist.cpp
  htmltaglist.cpp
  htmltag.cpp
  htmlparser.cpp
)

set(HTMLPARSER_HEADERS
  searchlist.hpp
  htmltaglist.hpp
  htmltag.hpp
  htmlparser.hpp
)

flex_target(htmlFlex html.l ${CMAKE_CURRENT_BINARY_DIR}/html.yy.cc
    COMPILE_FLAGS "-CF --header-file=${CMAKE_CURRENT_BINARY_DIR}/html.yy.hh")
flex_target(cssFlex css.l ${CMAKE_CURRENT_BINARY_DIR}/css.yy.cc
    COMPILE_FLAGS "-CF --header-file=${CMAKE_CURRENT_BINARY_DIR}/css.yy.hh")
bison_target(htmlBison html.y ${CMAKE_CURRENT_BINARY_DIR}/html.tab.cc)
bison_target(cssBison css.y ${CMAKE_CURRENT_BINARY_DIR}/css.tab.cc)
add_flex_bison_dependency(htmlFlex htmlBison)
add_flex_bison_dependency(cssFlex cssBison)

add_library(htmlParser SHARED
  ${HTMLPARSER_SRC}
  ${FLEX_htmlFlex_OUTPUTS}
  ${FLEX_cssFlex_OUTPUTS}
  ${BISON_htmlBison_OUTPUTS}
  ${BISON_cssBison_OUTPUTS}
)

#link htmlParser staticaly with flex
target_link_libraries(htmlParser ${FLEX_LIBRARIES})

set_target_properties(htmlParser PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS}"
                                            COMPILE_FLAGS_DEBUG "-Weffc++")


#install files
if(WIN32)  # for windows (dll = runtime)
  install (TARGETS htmlParser RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else(WIN32)
  install (TARGETS htmlParser LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif(WIN32)

install (FILES ${HTMLPARSER_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

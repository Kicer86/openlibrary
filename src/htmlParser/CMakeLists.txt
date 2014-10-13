project( htmlParser )
cmake_minimum_required(VERSION 2.8)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)

set(HTMLPARSER_SRC
    private/searchlist.cpp
    private/htmltaglist.cpp
    private/htmltag.cpp
    private/htmlparser.cpp
)

set(HTMLPARSER_HEADERS
    htmltag.hpp
    htmlparser.hpp
)

set(TEST_SRS
    private/tests/RunAllTests.cpp
    private/tests/htmlTag_tests.cpp
    private/tests/htmlTagList_tests.cpp
)

flex_target(htmlFlex private/html.l ${CMAKE_CURRENT_BINARY_DIR}/html.yy.cc
    COMPILE_FLAGS "-CF --header-file=${CMAKE_CURRENT_BINARY_DIR}/html.yy.hh")
flex_target(cssFlex private/css.l ${CMAKE_CURRENT_BINARY_DIR}/css.yy.cc
    COMPILE_FLAGS "-CF --header-file=${CMAKE_CURRENT_BINARY_DIR}/css.yy.hh")
bison_target(htmlBison private/html.y ${CMAKE_CURRENT_BINARY_DIR}/html.tab.cc)
bison_target(cssBison private/css.y ${CMAKE_CURRENT_BINARY_DIR}/css.tab.cc)
add_flex_bison_dependency(htmlFlex htmlBison)
add_flex_bison_dependency(cssFlex cssBison)

add_library(htmlParser SHARED ${HTMLPARSER_SRC} ${FLEX_htmlFlex_OUTPUTS} ${FLEX_cssFlex_OUTPUTS} ${BISON_htmlBison_OUTPUTS} ${BISON_cssBison_OUTPUTS} ${HTMLPARSER_HEADERS})
target_link_libraries(htmlParser ${FLEX_LIBRARIES} common)

#enable debug info when building debug
set_target_properties(htmlParser PROPERTIES COMPILE_DEFINITIONS $<$<CONFIG:Debug>:YYDEBUG=1>)

#if(TARGET htmlParserTests)
#    set_target_properties(htmlParserTests PROPERTIES COMPILE_DEFINITIONS_DEBUG YYDEBUG=1)
#endif(TARGET htmlParserTests)

#c++11 is needed here, turn it on
turnOnCpp11(htmlParser)

register_target(NAME htmlParser
                SOURCES ${HTMLPARSER_SRC} ${FLEX_htmlFlex_OUTPUTS} ${FLEX_cssFlex_OUTPUTS} ${BISON_htmlBison_OUTPUTS} ${BISON_cssBison_OUTPUTS}
                HEADERS ${HTMLPARSER_HEADERS})

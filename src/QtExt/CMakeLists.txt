
project( QtExt )
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif(POLICY CMP0020)

find_package(Qt5Widgets REQUIRED)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)

set(LIB_SRC
  qtext_choosefile.cpp
  qtext_progressbar.cpp
)

set(LIB_HEADERS
  qtext_choosefile.hpp
  qtext_progressbar.hpp
)

qt5_wrap_cpp(LIB_MOCFILES ${LIB_HEADERS})

add_library(QtExt SHARED ${LIB_SRC} ${LIB_MOCFILES} ${LIB_HEADERS})
target_link_libraries(QtExt Qt5::Widgets)

register_target(NAME QtExt
                SOURCES ${LIB_SRC} ${LIB_MOCFILES}
                HEADERS ${LIB_HEADERS})

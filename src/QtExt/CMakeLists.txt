
set(LIB_NAME QtExt)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

find_package(Qt4 4.6 REQUIRED QtGui QtCore REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${CMAKE_SOURCE_DIR}/src)

set(LIB_SRC
  qtext_choosefile.cpp
  qtext_progressbar.cpp
)

set(LIB_HEADERS
  qtext_choosefile.hpp
  qtext_progressbar.hpp
)

qt4_automoc(${LIB_SRC})
qt4_wrap_cpp(LIB_MOCFILES ${LIB_HEADERS})

register_library(${LIB_NAME} SOURCES ${LIB_SRC} ${LIB_MOCFILES} HEADERS ${LIB_HEADERS})
turnOnCpp11(${LIB_NAME})

target_link_libraries(${LIB_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

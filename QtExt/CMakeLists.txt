set(LIB_NAME QtExt)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

find_package(Qt4 4.6 REQUIRED QtGui QtCore)

include_directories(${QT_INCLUDES})

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

add_library(${LIB_NAME} SHARED ${LIB_SRC} ${LIB_MOCFILES} )
set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS}")

target_link_libraries(${LIB_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

#install files
if (WIN32)  # for windows (dll = runtime)
  install (TARGETS ${LIB_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else (WIN32)
  install (TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif(WIN32)

install (FILES ${LIB_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)


set(LIB_NAME QtExt)

project( ${LIB_NAME} )
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif(POLICY CMP0020)

find_package(Qt5Widgets)

if(Qt5Widgets_DIR)
	set(Qt5 TRUE)
else()
	set(Qt5 FALSE)
endif(Qt5Widgets_DIR)

if(NOT Qt5)
	find_package(Qt4 4.6 REQUIRED QtGui QtCore REQUIRED)

	include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${CMAKE_SOURCE_DIR}/src)
else()
	include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)
endif(NOT Qt5)

set(LIB_SRC
  qtext_choosefile.cpp
  qtext_progressbar.cpp
)

set(LIB_HEADERS
  qtext_choosefile.hpp
  qtext_progressbar.hpp
)

if(NOT Qt5)
	qt4_automoc(${LIB_SRC})
	qt4_wrap_cpp(LIB_MOCFILES ${LIB_HEADERS})
else()
	qt5_wrap_cpp(LIB_MOCFILES ${LIB_HEADERS})
endif()

register_library(${LIB_NAME} SOURCES ${LIB_SRC} ${LIB_MOCFILES} HEADERS ${LIB_HEADERS})
turnOnCpp11(${LIB_NAME})

message("${LIB_NAME}: using Qt5: ${Qt5}")    

if(Qt5)
	qt5_use_modules(${LIB_NAME} Widgets)
else()
	target_link_libraries(${LIB_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
endif()

cmake_minimum_required( VERSION 2.6 )
find_package ( Qt4 REQUIRED )
set ( CMAKE_BUILD_TYPE Release )

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})


set ( event_SRCS main.cpp	PortListener.cpp )

set ( event_MOCS PortListener.h )

qt4_automoc(event_SRCS)
qt4_wrap_cpp(MOCS ${event_MOCS})

add_executable ( event ${event_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( event  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} qextserialport)

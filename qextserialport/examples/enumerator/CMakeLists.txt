cmake_minimum_required( VERSION 2.6 )
find_package ( Qt4 REQUIRED )
set ( CMAKE_BUILD_TYPE Release )

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	)

set ( enumerator_SRCS main.cpp )

add_executable ( enumerator ${enumerator_SRCS})
target_link_libraries ( enumerator  
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        qextserialport)

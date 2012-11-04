project( qextserialport )
cmake_minimum_required( VERSION 2.6 )
find_package ( Qt4 REQUIRED )

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	)

set ( qextserialport_SRCS
	qextserialport.cpp
	)

set(LIBRARY_TYPE SHARED)
add_definitions(-DQEXTSERIALPORT_LIB)    #it's necessary on windows

if (UNIX AND NOT WIN32 AND NOT APPLE)
  message(STATUS "Compiling for Unix")
  set (qextserialport_SRCS ${qextserialport_SRCS} 
        posix_qextserialport.cpp
        qextserialenumerator_unix.cpp
      )
elseif(WIN32)
  message(STATUS "Compiling for Windows")
  set (qextserialport_SRCS ${qextserialport_SRCS} 
        win_qextserialport.cpp
        qextserialenumerator_win.cpp
      )
  add_definitions(-DWINVER=0x0501)
  set (EXTRA_LIBRARIES setupapi)
elseif(APPLE)
  message(STATUS "Compiling for Apple")
  set (qextserialport_SRCS ${qextserialport_SRCS} 
      posix_qextserialport.cpp
    )
endif(UNIX AND NOT WIN32 AND NOT APPLE)

set (qextserialport_MOCS
      qextserialport.h
      qextserialenumerator.h
    )
    
set (qextserialport_HDRS ${qextserialport_MOCS} qextserialport_global.h)
   
qt4_automoc(${qextserialport_SRCS})
qt4_wrap_cpp(MOCS ${qextserialport_MOCS})

add_library(qextserialport SHARED ${qextserialport_SRCS} ${MOCS})
target_link_libraries(qextserialport ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${EXTRA_LIBRARIES})

#install files
if ("${WIN32}")  # for windows (library=RUNTIME)
  install (TARGETS qextserialport RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else ("${WIN32}")
  install (TARGETS qextserialport LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif("${WIN32}")

install (FILES ${qextserialport_HDRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

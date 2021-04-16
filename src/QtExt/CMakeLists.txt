
project( QtExt )
cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Widgets)
if(Qt6_FOUND)
    message("Linking ${PROJECT_NAME} against Qt6")
else()
    find_package(Qt5 5.15 COMPONENTS REQUIRED Widgets)
    message("Linking ${PROJECT_NAME} against Qt5")
endif()

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)

set(LIB_SRC
  qtext_choosefile.cpp
  qtext_progressbar.cpp
)

set(LIB_HEADERS
  qtext_choosefile.hpp
  qtext_progressbar.hpp
)

qt_wrap_cpp(LIB_MOCFILES ${LIB_HEADERS})

add_library(QtExt SHARED ${LIB_SRC} ${LIB_MOCFILES} ${LIB_HEADERS})
target_link_libraries(QtExt Qt::Widgets)

register_target(NAME QtExt
                SOURCES ${LIB_SRC} ${LIB_MOCFILES}
                HEADERS ${LIB_HEADERS})

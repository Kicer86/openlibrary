project(math)

cmake_minimum_required( VERSION 2.6 )

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)

set(MATH_SRC
  crc32.cpp
  math.cpp
)

set(MATH_HEADERS
  crc32.hpp
  math.hpp
)


add_library(math SHARED ${MATH_SRC} )


set_target_properties(math PROPERTIES COMPILE_FLAGS "--std=c++0x ${LIB_CXXFLAGS} -masm=intel"
                                      COMPILE_FLAGS_DEBUG "-Weffc++")


#install files
if(WIN32)  # for windows (dll = runtime)
  install (TARGETS math RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else(WIN32)
  install (TARGETS math LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif(WIN32)

install (FILES ${MATH_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)


############################### tests ###############################

enable_testing()

#add_executable(TestTypes tests/types_test.cpp)
#target_link_libraries(TestTypes math utils)

#add_test(test_types ${CMAKE_CURRENT_BINARY_DIR}/TestTypes TestingTypes)

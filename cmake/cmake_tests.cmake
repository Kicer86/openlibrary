
function(runTests)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_custom_target(cmake_tests
                          COMMAND sh ./doTests.sh ${LIBRARIES_CMAKE}
                          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
    endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endfunction(runTests)
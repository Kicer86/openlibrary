
function(runTests)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message("Running cmake rules tests")
        execute_process(COMMAND sh ./doTests.sh
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
    endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endfunction(runTests)


function(runTests)
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_custom_target(performCMakeRulesChecks
                          COMMAND sh ./doTests.sh ${LIBRARIES_CMAKE}
                          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)

        add_dependencies(test performCMakeRulesChecks)
    endif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
endfunction(runTests)

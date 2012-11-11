
function(runTests)

    #as some unix commands are being used here - do not add anything on non unix systems
    if(UNIX)
        add_custom_target(performCMakeRulesChecks
                          COMMAND sh ./doTests.sh ${LIBRARIES_CMAKE}
                          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)

        add_dependencies(test performCMakeRulesChecks)
    endif(UNIX)
    
endfunction(runTests)


#rules for finding OpenLibrary
#based on http://www.vtk.org/Wiki/CMake:How_To_Find_Libraries
#and on FindQt4.cmake for Mageia Linux (components part)
#http://cmake.org/gitweb?p=cmake.git;a=blob;f=Modules/readme.txt

include(FindPackageHandleStandardArgs)

if(OpenLibrary_FIND_COMPONENTS)

    foreach(component ${OpenLibrary_FIND_COMPONENTS})
        STRING(TOUPPER ${component} comp_name)
        
        #try to load component's data
        set(OL_ENABLE_COMPONENT_${comp_name} 1)
        message("Finding component ${comp_name}")
        
    endforeach(component ${OpenLibrary_FIND_COMPONENTS})

endif(OpenLibrary_FIND_COMPONENTS)


#find common stuff
find_path(OPENLIBRARY_INCLUDE_DIRS OpenLibrary/common/debug.hpp)
find_library(OPENLIBRARY_LIBRARIES OpenLibrary/libcommon.so)

find_package_handle_standard_args(OpenLibrary DEFAULT_MSG OPENLIBRARY_INCLUDE_DIRS)

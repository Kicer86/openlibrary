
#rules for finding OpenLibrary
#based on http://www.vtk.org/Wiki/CMake:How_To_Find_Libraries
#and on FindQt4.cmake for Mageia Linux (components part)

include(FindPackageHandleStandardArgs)

if(OpenLibrary_FIND_COMPONENTS)

    foreach(component ${OpenLibrary_FIND_COMPONENTS})
        STRING(TOUPPER ${component} comp_name)
        
        #try to load component's data
        set(OL_ENABLE_COMPONENT_${TOUPPER} 1)
        
    endforeach(component ${OpenLibrary_FIND_COMPONENTS})

endif(OpenLibrary_FIND_COMPONENTS)


#find common stuff
find_path(OPENLIBRARY_INCLUDE_DIRS OpenLibrary/common/debug.hpp)

find_package_handle_standard_args(OpenLibrary DEFAULT_MSG OPENLIBRARY_INCLUDE_DIRS)
set ( HTMLPARSER_LIBRARIES htmlParser ) 
set ( MATH_LIBRARIES math ) 
set ( QTEXT_LIBRARIES QtExt ) 
set ( UTILS_LIBRARIES utils ) 

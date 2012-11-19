
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

find_package_handle_standard_args(OpenLibrary DEFAULT_MSG OPENLIBRARY_INCLUDE_DIRS)


if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_HTMLPARSER)

    find_path(OPEN_LIBRARY_HTMLPARSER_INCLUDE_DIRS NAMES searchlist.hpp htmltaglist.hpp htmltag.hpp htmlparser.hpp
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPEN_LIBRARY_HTMLPARSER_LIBRARIES NAMES htmlParser
                 HINTS ${OPENLIBRARY_INCLUDE_DIRS}/.. )
    
    find_package_handle_standard_args(OpenLibrary_HTMLPARSER DEFAULT_MSG
                                      OPEN_LIBRARY_HTMLPARSER_INCLUDE_DIRS OPEN_LIBRARY_HTMLPARSER_LIBRARIES)

    mark_as_advanced(OPEN_LIBRARY_HTMLPARSER_INCLUDE_DIRS OPEN_LIBRARY_HTMLPARSER_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_HTMLPARSER)



if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_MATH)

    find_path(OPEN_LIBRARY_MATH_INCLUDE_DIRS NAMES crc32.hpp math.hpp
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPEN_LIBRARY_MATH_LIBRARIES NAMES math
                 HINTS ${OPENLIBRARY_INCLUDE_DIRS}/.. )
    
    find_package_handle_standard_args(OpenLibrary_MATH DEFAULT_MSG
                                      OPEN_LIBRARY_MATH_INCLUDE_DIRS OPEN_LIBRARY_MATH_LIBRARIES)

    mark_as_advanced(OPEN_LIBRARY_MATH_INCLUDE_DIRS OPEN_LIBRARY_MATH_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_MATH)



if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_QTEXT)

    find_path(OPEN_LIBRARY_QTEXT_INCLUDE_DIRS NAMES qtext_choosefile.hpp qtext_progressbar.hpp
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPEN_LIBRARY_QTEXT_LIBRARIES NAMES QtExt
                 HINTS ${OPENLIBRARY_INCLUDE_DIRS}/.. )
    
    find_package_handle_standard_args(OpenLibrary_QTEXT DEFAULT_MSG
                                      OPEN_LIBRARY_QTEXT_INCLUDE_DIRS OPEN_LIBRARY_QTEXT_LIBRARIES)

    mark_as_advanced(OPEN_LIBRARY_QTEXT_INCLUDE_DIRS OPEN_LIBRARY_QTEXT_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_QTEXT)



if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_UTILS)

    find_path(OPEN_LIBRARY_UTILS_INCLUDE_DIRS NAMES copy_ptr.hpp cpuinfo.hpp
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPEN_LIBRARY_UTILS_LIBRARIES NAMES utils
                 HINTS ${OPENLIBRARY_INCLUDE_DIRS}/.. )
    
    find_package_handle_standard_args(OpenLibrary_UTILS DEFAULT_MSG
                                      OPEN_LIBRARY_UTILS_INCLUDE_DIRS OPEN_LIBRARY_UTILS_LIBRARIES)

    mark_as_advanced(OPEN_LIBRARY_UTILS_INCLUDE_DIRS OPEN_LIBRARY_UTILS_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_UTILS)


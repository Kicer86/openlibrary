
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


if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_HTMLPARSER)

    get_filename_component(OL_LIB_PATH ${OPENLIBRARY_LIBRARIES} PATH)

    find_path(OPENLIBRARY_HTMLPARSER_INCLUDE_DIRS NAMES OpenLibrary/htmlParser/htmltag.hpp OpenLibrary/htmlParser/htmltaglist.hpp OpenLibrary/htmlParser/searchlist.hpp OpenLibrary/htmlParser/libhtmlParser.so
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPENLIBRARY_HTMLPARSER_LIBRARIES NAMES htmlParser
                 HINTS ${OL_LIB_PATH} )
    
    find_package_handle_standard_args(OpenLibrary_HTMLPARSER DEFAULT_MSG
                                      OPENLIBRARY_HTMLPARSER_INCLUDE_DIRS OPENLIBRARY_HTMLPARSER_LIBRARIES)

    mark_as_advanced(OPENLIBRARY_HTMLPARSER_INCLUDE_DIRS OPENLIBRARY_HTMLPARSER_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_HTMLPARSER)



if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_MATH)

    get_filename_component(OL_LIB_PATH ${OPENLIBRARY_LIBRARIES} PATH)

    find_path(OPENLIBRARY_MATH_INCLUDE_DIRS NAMES OpenLibrary/math/crc32.hpp OpenLibrary/math/libmath.so
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPENLIBRARY_MATH_LIBRARIES NAMES math
                 HINTS ${OL_LIB_PATH} )
    
    find_package_handle_standard_args(OpenLibrary_MATH DEFAULT_MSG
                                      OPENLIBRARY_MATH_INCLUDE_DIRS OPENLIBRARY_MATH_LIBRARIES)

    mark_as_advanced(OPENLIBRARY_MATH_INCLUDE_DIRS OPENLIBRARY_MATH_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_MATH)



if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_QTEXT)

    get_filename_component(OL_LIB_PATH ${OPENLIBRARY_LIBRARIES} PATH)

    find_path(OPENLIBRARY_QTEXT_INCLUDE_DIRS NAMES OpenLibrary/QtExt/qtext_choosefile.hpp OpenLibrary/QtExt/libQtExt.so
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPENLIBRARY_QTEXT_LIBRARIES NAMES QtExt
                 HINTS ${OL_LIB_PATH} )
    
    find_package_handle_standard_args(OpenLibrary_QTEXT DEFAULT_MSG
                                      OPENLIBRARY_QTEXT_INCLUDE_DIRS OPENLIBRARY_QTEXT_LIBRARIES)

    mark_as_advanced(OPENLIBRARY_QTEXT_INCLUDE_DIRS OPENLIBRARY_QTEXT_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_QTEXT)



if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_UTILS)

    get_filename_component(OL_LIB_PATH ${OPENLIBRARY_LIBRARIES} PATH)

    find_path(OPENLIBRARY_UTILS_INCLUDE_DIRS NAMES OpenLibrary/utils/copy_ptr.hpp OpenLibrary/utils/libutils.so
              HINTS ${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPENLIBRARY_UTILS_LIBRARIES NAMES utils
                 HINTS ${OL_LIB_PATH} )
    
    find_package_handle_standard_args(OpenLibrary_UTILS DEFAULT_MSG
                                      OPENLIBRARY_UTILS_INCLUDE_DIRS OPENLIBRARY_UTILS_LIBRARIES)

    mark_as_advanced(OPENLIBRARY_UTILS_INCLUDE_DIRS OPENLIBRARY_UTILS_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_UTILS)


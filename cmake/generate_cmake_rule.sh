#!/bin/bash

project_name=$1
lib=$2

#read all headers - all arguments but two first
shift 3
header=$@

echo "

if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_$project_name)

    find_path(OPEN_LIBRARY_${project_name}_INCLUDE_DIRS NAMES $header
              HINTS \${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPEN_LIBRARY_${project_name}_LIBRARIES NAMES $lib
                 HINTS \${OPENLIBRARY_INCLUDE_DIRS}/.. )
    
    find_package_handle_standard_args(OpenLibrary_${project_name} DEFAULT_MSG
                                      OPEN_LIBRARY_${project_name}_INCLUDE_DIRS OPEN_LIBRARY_${project_name}_LIBRARIES)

    mark_as_advanced(OPEN_LIBRARY_${project_name}_INCLUDE_DIRS OPEN_LIBRARY_${project_name}_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_$project_name)
"

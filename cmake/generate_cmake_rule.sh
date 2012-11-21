#!/bin/bash

project_name=$1
lib=$2

#read all headers - all arguments but two first
shift 3
header=$@

echo "

if(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_$project_name)

    get_filename_component(OL_LIB_PATH \${OPENLIBRARY_LIBRARIES} PATH)

    find_path(OPENLIBRARY_${project_name}_INCLUDE_DIRS NAMES $header
              HINTS \${OPENLIBRARY_INCLUDE_DIRS})
    
    find_library(OPENLIBRARY_${project_name}_LIBRARIES NAMES $lib
                 HINTS \${OL_LIB_PATH} )
    
    find_package_handle_standard_args(OpenLibrary_${project_name} DEFAULT_MSG
                                      OPENLIBRARY_${project_name}_INCLUDE_DIRS OPENLIBRARY_${project_name}_LIBRARIES)

    mark_as_advanced(OPENLIBRARY_${project_name}_INCLUDE_DIRS OPENLIBRARY_${project_name}_LIBRARIES)

endif(OPENLIBRARY_FOUND AND OL_ENABLE_COMPONENT_$project_name)
"

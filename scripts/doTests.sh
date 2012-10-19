#!/bin/bash
#make sure, that generated rules are ok

retpath=`pwd`
cd ../build

#1 test: cmake should generate rules for cmake variables for each of registered libraries
registerd_libs=`grep OPENLIBRARY_REGISTERED_LIBRARIES CMakeCache.txt | sed -e "s/OPENLIBRARY_REGISTERED_LIBRARIES:INTERNAL=//" -e "s/;/ /g"`

for lib in $registerd_libs; do

done

cd $retpath

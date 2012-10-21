#!/bin/bash
#make sure, that generated rules are ok

echo "Staring CMake's rules check"

libraries_cmake=$1

retpath=`pwd`
cd ../build

#before performing tests, make sure all targets exist
make libraries.cmake

#1 test: cmake should generate rules for cmake variables for each of registered libraries
registerd_libs=`grep OPENLIBRARY_REGISTERED_LIBRARIES CMakeCache.txt | sed -e "s/OPENLIBRARY_REGISTERED_LIBRARIES:INTERNAL=//" -e "s/;/ /g"`

echo "performing #1 test"
for lib in $registerd_libs; do
    echo "removing $lib file"
    rm -f $lib

    echo "making sure that $lib can be generated:"
    filename=`basename ${lib}`
    make "$filename"_variables

    exit_code=$?

    if [ $exit_code -eq 0 ]; then
        echo "ok!"
    else
        echo "failed to generate file $lib"
        exit 1;
    fi;
done


echo "performing #2 test"
for lib in $registerd_libs; do
    echo "touching $lib file"
    touch $lib

    echo "making sure that libraries.cmake will be regenerated:"
    filemodtime=`stat -c %Y $libraries_cmake`
    sleep 1                                            #make sure we will wait at least 1 second
    make libraries.cmake
    filemodtime2=`stat -c %Y $libraries_cmake`

    if [ $filemodtime -ne $filemodtime2 ]; then
        echo "ok!"
    else
        echo "file libraries.cmake has not regenerated"
        exit 1;
    fi;
done


echo "performing #3 test"
for lib in $registerd_libs; do

    echo "making sure that $lib will not be regenerated without a reason"
    filemodtime=`stat -c %Y $lib`
    filename=`basename ${lib}`
    make ${filename}_variables
    filemodtime2=`stat -c %Y $lib`

    if [ $filemodtime -eq $filemodtime2 ]; then
        echo "ok!"
    else
        echo "file $lib was regenerated without any reason"
        exit 1;
    fi;
done

echo "performing #4 test"
echo "making sure that libraries.cmake will not be regenerated without a reason"
    filemodtime=`stat -c %Y $libraries_cmake`
    make libraries.cmake
    filemodtime2=`stat -c %Y $libraries_cmake`

    if [ $filemodtime -eq $filemodtime2 ]; then
        echo "ok!"
    else
        echo "file libraries.cmake was regenerated without any reason"
        exit 1;
    fi;

cd $retpath

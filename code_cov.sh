
#!/bin/bash

#arguments:
# 1. path to test's executable file
# 2. path to library linked with test
# 3. ... source files to analyze

if [ $# -lt 3 ]; then
  echo "Not enought parameters"
  exit 1;
fi

TEST_BIN=$1
LIBRARY_BIN=$2
SOURCES=${*:3}

echo "Testing code coverage for sources: $SOURCES"

objs=""
#add .o to sources
for f in ${SOURCES}; do
    fname=`basename $f`
    objs="$objs $fname.o"
done

gcda_path=`strings ${TEST_BIN} | sed -n -e '/\.gcda$/p' | sed -e '1!d'`
gcda_path=`dirname $gcda_path`

gcda_extra_path=`strings ${LIBRARY_BIN} | sed -n -e '/\.gcda$/p' | sed -e '1!d'`
gcda_extra_path=`dirname $gcda_extra_path`

if [ "$gcda_path" != "" ]; then

    echo "gcov -o $gcda_path -o $gcda_extra_path $objs"
    gcov -o $gcda_path -o $gcda_extra_path $objs

fi

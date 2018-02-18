#!/bin/bash

echo "**********************************"
echo "building $1 with" 
echo "$CXX using $BUILDCHAIN"
echo "**********************************"

cd /usr/local/src/$1

if [ "$BUILDCHAIN" == "make" ] 
then
    make clean
    make -e test
    make clean
    make -e install
else
    mkdir -p build
    cd build
    cmake .. -DCMAKE_CXX_COMPILER=$CXX 
    cat ./CMakeFiles/after_test.dir/flags.make
    cat ./CMakeFiles/after_test.dir/link.txt
    make
    make test
    make install
fi


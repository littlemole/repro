#!/bin/bash
set -e

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
    make
    make test
    make install
fi


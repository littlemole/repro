# This is a comment
FROM ubuntu:18.04
MAINTAINER me <little.mole@oha7.org>

# std dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
build-essential g++ libgtest-dev cmake git \
pkg-config valgrind sudo joe wget \
clang libc++-dev libc++abi-dev

ARG CXX=g++
ENV CXX=${CXX}

# compile gtest with given compiler
ADD ./docker/gtest.sh /usr/local/bin/gtest.sh
RUN /usr/local/bin/gtest.sh

# add repro sources
RUN mkdir -p /usr/local/src/reprocpp
ADD . /usr/local/src/reprocpp

ARG BUILDCHAIN=make
ENV BUILDCHAIN=${BUILDCHAIN}

# make a repro build, test and install
ADD ./docker/build.sh /usr/local/bin/build.sh 
RUN /usr/local/bin/build.sh reprocpp



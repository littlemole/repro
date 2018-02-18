# This is a comment
FROM ubuntu:16.04
MAINTAINER me <little.mole@oha7.org>

# std dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential g++ \
libgtest-dev cmake git pkg-config valgrind sudo joe wget

# clang++-5.0 dependencies
RUN echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-5.0 main" >> /etc/apt/sources.list
RUN wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|apt-key add -

RUN DEBIAN_FRONTEND=noninteractive apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
clang-5.0 lldb-5.0 lld-5.0 libc++-dev libc++abi-dev

# hack for gtest with clang++-5.0
RUN ln -s /usr/include/c++/v1/cxxabi.h /usr/include/c++/v1/__cxxabi.h
RUN ln -s /usr/include/libcxxabi/__cxxabi_config.h /usr/include/c++/v1/__cxxabi_config.h

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



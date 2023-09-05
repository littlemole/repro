# This is a comment
FROM ubuntu:22.04
MAINTAINER me <little.mole@oha7.org>

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
  git sudo joe wget psmisc \
  build-essential g++ pkg-config valgrind \
  libgtest-dev clang libc++-dev libc++abi-dev \
  libboost-dev libboost-system-dev zlib1g-dev \
  cmake


ARG CXX=g++
ENV CXX=${CXX}

# add repro sources
RUN mkdir -p /usr/local/src/reprocpp
ADD . /usr/local/src/reprocpp

ADD ./docker/build.sh /usr/local/bin/build.sh
ADD ./docker/gtest.sh /usr/local/bin/gtest.sh

ARG BUILDCHAIN=make
ENV BUILDCHAIN=${BUILDCHAIN}

ARG TS=
ENV TS=${TS}

RUN /usr/local/bin/gtest.sh

# make a repro build, test and install
RUN /usr/local/bin/build.sh reprocpp



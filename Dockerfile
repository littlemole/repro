# This is a comment
FROM littlemole/devenv_gpp_make
MAINTAINER me <little.mole@oha7.org>

ARG CXX=g++
ENV CXX=${CXX}

# add repro sources
RUN mkdir -p /usr/local/src/reprocpp
ADD . /usr/local/src/reprocpp

ARG BUILDCHAIN=make
ENV BUILDCHAIN=${BUILDCHAIN}

ARG TS=
ENV TS=${TS}

# make a repro build, test and install
RUN /usr/local/bin/build.sh reprocpp



############################################
# preamble
############################################

cmake_minimum_required(VERSION 3.22)

project(reprocpp)
set(VERSION 0.0.29)
include(CTest)


############################################
# c++ std
############################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


############################################
# debug mode produces libs with d suffix
############################################

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()


############################################
# basic dependecies
############################################

find_package(GTest REQUIRED)


############################################
# clang support
############################################

set(LINKERFLAGS "")
set(STDLIB "")
set(FLAGS "-D_RESUMABLE_FUNCTIONS_SUPPORTED")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(STDLIB "c++abi")
    set(LINKERFLAGS "-stdlib=libc++ -fcoroutines-ts")
    set(FLAGS "-stdlib=libc++ -fcoroutines-ts -D_RESUMABLE_FUNCTIONS_SUPPORTED")
endif()


############################################
# OS support (Linux/Windows)
############################################

IF (WIN32)

    set(OSLIBS "Ws2_32" "Rpcrt4")
    set(BUILDFLAGS " ${FLAGS}")
    set(LIBS ${GTEST_LIBRARIES} ${STDLIB}  )

 ELSEIF (UNIX)

    set(OSLIBS "pthread")
    set(BUILDFLAGS " ${FLAGS}")
    set(LIBS ${GTEST_LIBRARIES} ${STDLIB}  )

ENDIF ()


############################################
# set linker and compiler flags
############################################

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINKERFLAGS}" )
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -DMOL_PROMISE_DEBUG")
set(CMAKE_CXX_FLAGS  	    "${CMAKE_CXX_FLAGS} ${BUILDFLAGS}")


############################################
# include directories
############################################

include_directories(include)
include_directories(${GTEST_INCLUDE_DIR})


############################################
# sources
############################################

file(GLOB_RECURSE HEADERS "include/reprocpp/*.h")


############################################
# lib target
############################################

add_library(reprocpp INTERFACE)
target_include_directories(reprocpp INTERFACE include/)


############################################
# tests
############################################

add_executable(basic_test t/test.cpp)
add_executable(after_test t/after.cpp)


############################################
# libs for tests
############################################

target_link_libraries(basic_test ${LIBS} ${OSLIBS} )
target_link_libraries(after_test ${LIBS} ${OSLIBS} )


############################################
# run tests
############################################

add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME AfterTest COMMAND after_test)


############################################
# install rules
############################################

install( DIRECTORY "include/${CMAKE_PROJECT_NAME}/" DESTINATION "include/${CMAKE_PROJECT_NAME}/" )
install(FILES reprocpp.pc DESTINATION lib/pkgconfig)


############################################
# Create and install package configuration 
# and version files.
############################################

configure_file(
   ${reprocpp_SOURCE_DIR}/reprocpp-config.cmake.in
   ${reprocpp_BINARY_DIR}/reprocpp-config.cmake @ONLY)

configure_file(
   ${reprocpp_SOURCE_DIR}/reprocpp-config-version.cmake.in
   ${reprocpp_BINARY_DIR}/reprocpp-config-version.cmake @ONLY)

install(FILES ${reprocpp_BINARY_DIR}/reprocpp-config.cmake
         ${reprocpp_BINARY_DIR}/reprocpp-config-version.cmake
         DESTINATION lib/reprocpp-${VERSION})


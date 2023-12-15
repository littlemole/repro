############################################
# preamble
############################################

cmake_minimum_required(VERSION 3.22)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(reprocpp)
set(VERSION 0.0.29)
include(CTest)

option(WITH_TEST "run tests (default:on)" ON)

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
# clang support
############################################

set(LINKERFLAGS "")
set(STDLIB "")

if(WIN32)
    set(FLAGS "/W3")
else()
    set(FLAGS "-Wall -Wextra")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(STDLIB "c++abi")
    set(LINKERFLAGS "-stdlib=libc++ -fcoroutines-ts")
    set(FLAGS "-stdlib=libc++ -fcoroutines-ts ")
endif()


############################################
# OS support (Linux/Windows)
############################################

IF (WIN32)

    find_package(GTest REQUIRED)

    set(OSLIBS "Ws2_32" "Rpcrt4")
    set(LIBS ${GTEST_LIBRARIES} ${STDLIB}  )

 ELSEIF (UNIX)

    set(OSLIBS "pthread")
    set(LIBS ${STDLIB}  )

ENDIF ()


############################################
# set linker and compiler flags
############################################

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINKERFLAGS}" )
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -DMOL_PROMISE_DEBUG")
set(CMAKE_CXX_FLAGS  	    "${CMAKE_CXX_FLAGS} ${FLAGS}")


############################################
# gtest 
############################################
if(UNIX AND WITH_TEST)

    include(FetchContent)

    FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    #FetchContent_MakeAvailable(googletest)

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    set(LIBS "gtest" ${LIBS})
endif()



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

IF(WITH_TEST)

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

endif()

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

############################################
# packaging
############################################

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(package)

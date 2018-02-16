cmake_minimum_required(VERSION 2.8.9)
project(reprocpp)
include(CTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


############################################
# OS support
############################################

# default to UNIX

IF (WIN32)
    find_package(GTest REQUIRED)
	set(OSLIBS "Ws2_32" "Rpcrt4")
	set(DEBUGFLAGS "-DMOL_PROMISE_DEBUG -await")
ELSEIF(UNIX)

    set(OSLIBS "pthread")
    find_package(GTest REQUIRED)
    find_package(PkgConfig)
    set(DEBUGFLAGS "-g -DMOL_PROMISE_DEBUG ${GTEST_CFLAGS_OTHER}")
ENDIF ()

############################################
# macros
############################################

macro(AddCompilerFlags target flags)
    get_target_property(CF ${target} COMPILE_FLAGS)
    if(CF STREQUAL "CF-NOTFOUND")
        SET(CF "") # set to empty string
    else()
        SET(CF "${CF} ") # a space to cleanly separate from existing content
    endif()

	SET(CF "${CF} ${flags}" )
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS ${CF} )
endmacro()

############################################
# includes
############################################

include_directories(include)
include_directories(${GTEST_INCLUDE_DIR})

file(GLOB HEADERS "include/reprocpp/*.h")

############################################
# target library
############################################

add_library(reprocpp INTERFACE)
target_include_directories(reprocpp INTERFACE include/)

############################################
# tests
############################################

add_executable(basic_test t/test.cpp)
add_executable(after_test t/after.cpp)

AddCompilerFlags(basic_test ${DEBUGFLAGS})
AddCompilerFlags(after_test ${DEBUGFLAGS})

target_link_libraries(basic_test ${GTEST_LIBRARY} ${OSLIBS} )
target_link_libraries(after_test ${GTEST_LIBRARY} ${OSLIBS} )

add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME AfterTest COMMAND after_test)
 
############################################
# install
############################################

install(FILES ${HEADERS} DESTINATION include/reprocpp)

install(FILES reprocpp.pc DESTINATION lib/pkgconfig)

cmake_minimum_required(VERSION 2.8.9)
project(reprocpp)
include(CTest)

IF (WIN32)
find_package(gtest REQUIRED)
ENDIF ()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

file(GLOB HEADERS "include/reprocpp/*.h")

add_library(reprocpp INTERFACE)
target_include_directories(reprocpp INTERFACE include/)

add_executable(basic_test t/test.cpp)
add_executable(after_test t/after.cpp)

IF (UNIX)

target_link_libraries(basic_test gtest reprocpp pthread)
target_link_libraries(after_test gtest reprocpp pthread)

ELSEIF (WIN32)

target_link_libraries(basic_test ${GTEST_LIBRARY} reprocpp Ws2_32 Rpcrt4 )
target_link_libraries(after_test ${GTEST_LIBRARY} reprocpp Ws2_32 Rpcrt4 )

ENDIF ()

add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME AfterTest COMMAND after_test)
 
install(FILES ${HEADERS} DESTINATION include/reprocpp)

install(FILES reprocpp.pc DESTINATION lib/pkgconfig)

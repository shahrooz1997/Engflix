cmake_minimum_required(VERSION 3.10)
project(English_Teacher)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Werror -Wconversion")

file(GLOB SOURCES *.cpp)
set(LIB_SOURCES ${SOURCES})
set(MAIN_SOURCES ${CMAKE_SOURCE_DIR}/main.cpp)
list(REMOVE_ITEM LIB_SOURCES ${MAIN_SOURCES})

include_directories(/usr/include/dbus-1.0 ${CMAKE_BINARY_DIR})
link_libraries(curses pthread protobuf)
add_library(engFlix_lib ${LIB_SOURCES})

link_libraries(engFlix_lib)
add_executable(English_Teacher ${CMAKE_SOURCE_DIR}/main.cpp)

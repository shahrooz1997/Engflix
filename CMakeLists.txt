cmake_minimum_required(VERSION 3.17)
project(English_Teacher)

set(CMAKE_CXX_STANDARD 11)

add_executable(English_Teacher main.cpp One_subtitle.cpp One_subtitle.h Util.h Util.cpp Subtitle.cpp Subtitle.h VLC_interface.cpp VLC_interface.h)
cmake_minimum_required(VERSION 3.17)
project(English_Teacher)

set(CMAKE_CXX_STANDARD 11)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

#add_executable(English_Teacher main.cpp One_subtitle.cpp One_subtitle.h Util.h Util.cpp Subtitle.cpp Subtitle.h
#        VLC_interface.cpp VLC_interface.h Handler.cpp Handler.h Application.cpp)

add_executable(English_Teacher main.cpp One_subtitle.cpp Util.cpp Subtitle.cpp
        VLC_interface.cpp Handler.cpp Application.cpp Main_frame.cpp)

target_link_libraries(English_Teacher curses pthread)
target_link_libraries(English_Teacher ${wxWidgets_LIBRARIES})
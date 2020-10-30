//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_VLC_INTERFACE_H
#define ENGLISH_TEACHER_VLC_INTERFACE_H

#include "Util.h"
#include <string>


enum State{
    Playing,
    Paused,
    Stopped
};

class VLC_interface{
public:
    VLC_interface() = default;
    explicit VLC_interface(const std::string& file_path);
    ~VLC_interface();
    VLC_interface(const VLC_interface& orig) = delete;

    void close();

    static void play_pause();
    static void play();
    static void pause();
    static State get_state();

    static void seek(const std::chrono::milliseconds& dur);
    static void seek(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>& tp);
    static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> tell();
};


#endif //ENGLISH_TEACHER_VLC_INTERFACE_H

//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_VLC_INTERFACE_H
#define ENGLISH_TEACHER_VLC_INTERFACE_H

#include "Util.h"

using namespace std;
using namespace chrono;

enum State{
    Playing,
    Paused,
    Stopped
};

class VLC_interface{
public:
    VLC_interface() = default;
    explicit VLC_interface(const string& file_path);
    ~VLC_interface();
    VLC_interface(const VLC_interface& orig) = delete;

    void close();

    static void play_pause();
    static State get_state();

    static void seek(const milliseconds& dur);
    static void seek(const time_point<steady_clock, milliseconds>& tp);
    static time_point<steady_clock, milliseconds> tell();
};


#endif //ENGLISH_TEACHER_VLC_INTERFACE_H

//
// Created by shahrooz on 10/29/20.
//

#include "VLC_interface.h"
#include <fstream>

using namespace std;
using namespace chrono;

// Todo: Change this class to use dbus API

VLC_interface::VLC_interface(const string& file_path){
    // Todo: launch VLC with the video in the path
}

VLC_interface::VLC_interface(){
}

VLC_interface::~VLC_interface(){
    // Todo: close vlc only if it is open
    this->close();
}

void VLC_interface::close(){
    execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Quit"});
}

void VLC_interface::play_pause(){
    execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.PlayPause"});
}

void VLC_interface::play(){
    execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Play"});
}

void VLC_interface::pause(){
    execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Pause"});
}

void VLC_interface::seek(const milliseconds& dur){
    execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Seek",
                          string("int64:") + to_string(microseconds(dur).count())});
}

// Todo: set position instead of the following function
void VLC_interface::seek(const time_point<steady_clock, milliseconds>& tp){
    time_point<steady_clock, milliseconds> pos = tell();
    seek(tp - pos);
}

time_point<steady_clock, milliseconds> VLC_interface::tell(){
    string temp = execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties.Get",
                          "string:org.mpris.MediaPlayer2.Player", "string:Position"});

    temp = temp.substr(temp.find("int64 ", temp.find("\n") + 1) + 6);
    uint64_t position = stoull(temp) / 1000; // Time in milliseconds

    return time_point<steady_clock, milliseconds>(milliseconds(position));
}


State VLC_interface::get_state(){
    string temp = execute("dbus-send", {"--print-reply", "--type=method_call", "--dest=org.mpris.MediaPlayer2.vlc",
                          "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties.Get",
                          "string:org.mpris.MediaPlayer2.Player", "string:PlaybackStatus"});

    temp = temp.substr(temp.find("string ", temp.find("\n") + 1) + 8);
    temp.erase(temp.size() - 1);
    State ret;
    if(temp == "Paused"){
        ret = Paused;
    }
    else if(temp == "Stopped"){
        ret = Stopped;
    }
    else if(temp == "Playing"){
        ret = Playing;
    }
    else{
        assert(false);
    }

    return ret;
}



//
// Created by shahrooz on 10/29/20.
//

#include "VLC_interface.h"
#include <fstream>

// Todo: Change this class to use dbus API

VLC_interface::VLC_interface(const string& file_path){
    // Todo: launch VLC with the video in the path
}

VLC_interface::~VLC_interface(){
    // Todo: close vlc only if it is open
//    this->close();
}

void VLC_interface::close(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.mpris.MediaPlayer2.Quit ";
    command += "> /dev/null";
    system(command.c_str());
}

void VLC_interface::play_pause(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.mpris.MediaPlayer2.Player.PlayPause ";
    command += "> /dev/null";
    system(command.c_str());
}

void VLC_interface::seek(const milliseconds& dur){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.mpris.MediaPlayer2.Player.Seek ";
    command += "int64:\"";
    command += to_string(microseconds(dur).count()); // VLC time is in microseconds
    command += "\" ";
    command += "> /dev/null";
    system(command.c_str());
}

void VLC_interface::seek(const time_point<steady_clock, milliseconds>& tp){
    time_point<steady_clock, milliseconds> pos = tell();
    seek(tp - pos);
}

time_point<steady_clock, milliseconds> VLC_interface::tell(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.freedesktop.DBus.Properties.Get string:\"org.mpris.MediaPlayer2.Player\" string:\"Position\" ";
    command += "> ../data/Position.txt";
    system(command.c_str());

    ifstream in("../data/Position.txt", ios::in);
    string temp;
    getline(in, temp);
    getline(in, temp);
    temp = temp.substr(temp.find("int64 ") + 6);
    uint64_t position = stoull(temp) / 1000; // Time in milliseconds

    return time_point<steady_clock, milliseconds>(milliseconds(position));
}


State VLC_interface::get_state(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.freedesktop.DBus.Properties.Get string:\"org.mpris.MediaPlayer2.Player\" string:\"PlaybackStatus\" ";
    command += "> ../data/getStatus.txt";
    system(command.c_str());

    ifstream in("../data/getStatus.txt", ios::in);
    string temp;
    getline(in, temp);
    getline(in, temp);
    temp = temp.substr(temp.find("string ") + 8);
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

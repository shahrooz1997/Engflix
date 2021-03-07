//
// Created by shahrooz on 10/29/20.
//

#include "VLC_interface.h"
#include <fstream>

using namespace std;
using namespace chrono;

// Todo: Change this class to use dbus API

VLC_interface::VLC_interface(const string& file_path){
    system("mkdir -p " TEMP_FILES_ADDR);
    // Todo: launch VLC with the video in the path
}

VLC_interface::VLC_interface(){
    system("mkdir -p " TEMP_FILES_ADDR);
}

VLC_interface::~VLC_interface(){
    // Todo: close vlc only if it is open
//    this->close();
    system("rm -rf " TEMP_FILES_ADDR);
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

void VLC_interface::play(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.mpris.MediaPlayer2.Player.Play ";
    command += "> /dev/null";
    system(command.c_str());
}

void VLC_interface::pause(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.mpris.MediaPlayer2.Player.Pause ";
    command += "> /dev/null";
    system(command.c_str());
}

void VLC_interface::seek(const milliseconds& dur){
//    DPRINTF(DEBUG_VLC_interface, "bbbbbb\n");
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.mpris.MediaPlayer2.Player.Seek ";
    command += "int64:\"";
    command += to_string(microseconds(dur).count()); // VLC time is in microseconds
    command += "\" ";
    command += "> /dev/null";
    system(command.c_str());
}

// Todo: set position instead of the following function
void VLC_interface::seek(const time_point<steady_clock, milliseconds>& tp){
//    DPRINTF(DEBUG_VLC_interface, "aaaaaa\n");
    time_point<steady_clock, milliseconds> pos = tell();
    seek(tp - pos);
}

time_point<steady_clock, milliseconds> VLC_interface::tell(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.freedesktop.DBus.Properties.Get string:\"org.mpris.MediaPlayer2.Player\" string:\"Position\" ";
    command += "> " TEMP_FILES_ADDR "Position_";
    string unique_value = to_string(std::chrono::system_clock::now().time_since_epoch().count());
    command += unique_value;
    command += ".txt";
    system(command.c_str());

    string file_name = TEMP_FILES_ADDR "Position_";
    file_name += unique_value;
    file_name += ".txt";
    ifstream in(file_name, ios::in);
    string temp;
    getline(in, temp);
//    DPRINTF(DEBUG_VLC_interface, "1|%s|\n", temp.c_str());
    getline(in, temp);
//    DPRINTF(DEBUG_VLC_interface, "2|%s|\n", temp.c_str());
    temp = temp.substr(temp.find("int64 ") + 6);
    uint64_t position = stoull(temp) / 1000; // Time in milliseconds

    command = "rm -rf ";
    command += file_name;
    system(command.c_str());

    return time_point<steady_clock, milliseconds>(milliseconds(position));
}


State VLC_interface::get_state(){
    string command = "dbus-send --print-reply --type=method_call --dest=org.mpris.MediaPlayer2.vlc ";
    command += "/org/mpris/MediaPlayer2 ";
    command += "org.freedesktop.DBus.Properties.Get string:\"org.mpris.MediaPlayer2.Player\" string:\"PlaybackStatus\" ";
    command += "> " TEMP_FILES_ADDR "getStatus.txt";
    system(command.c_str());

    ifstream in(TEMP_FILES_ADDR "getStatus.txt", ios::in);
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



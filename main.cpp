#include <iostream>
#include "Subtitle.h"
#include "VLC_interface.h"
#include <iostream>


// Todo: add a recorder
// Todo: playback the recorded voice with a mute playback of the video
int main(){

    VLC_interface vlc;

    vlc.seek(seconds(5));
    vlc.seek(time_point<steady_clock, minutes>(minutes(5)));
    cout << vlc.tell().time_since_epoch().count() << endl;
    cout << vlc.get_state() << endl;
    vlc.play_pause();

//    vlc.close();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#include <iostream>
#include "Handler.h"


// Todo: add a recorder
// Todo: playback the recorded voice with a mute playback of the video
int main(){

    Handler::get_instance("/home/shahrooz/Videos/Boss_Baby/S01/The.Boss.Baby.Back.in.Business.S01E01.WEB.srt")->start();
    
    return 0;
}

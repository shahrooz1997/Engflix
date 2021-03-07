#include <iostream>
#include "Handler.h"

using namespace std;

// Todo: add a recorder
// Todo: playback the recorded voice with a mute playback of the video
int main(int argc, char* argv[]){

    if(argc < 2){
        cerr << "Usage: " << argv[0] << " [subtitle path]" << endl;
        return 1;
    }

    Handler::get_instance(argv[1])->start();

    return 0;
}

#include <iostream>
#include "Handler.h"

using namespace std;

// Todo: add a recorder
// Todo: playback the recorded voice with a mute playback of the video
// Todo: Add another argument as the start time
int main(int argc, char *argv[]) {

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " [subtitle path]" << endl;
    return 1;
  }

  Handler::get_instance(argv[1])->start();
//  Handler::get_instance("/home/shahrooz/Videos/Chucky2021/Chucky.S01E01.720p.WEB_.x265-MiNX.srt")->start();

  // Clean up
  Handler::del_instance();

  return 0;
}

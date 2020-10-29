#include <iostream>
#include "Subtitle.h"
#include <iostream>



int main(){

    Subtitle subtitle("V - 2x10 - TBA.HDTV.2HD.en.srt");

    cout << subtitle.get_subtitle().get_end_time().time_since_epoch().count() << endl;
    cout << subtitle.get_subtitle().get_end_time().time_since_epoch().count() << endl;
    cout << subtitle.get_subtitle().get_end_time().time_since_epoch().count() << endl;
    cout << subtitle.get_subtitle().get_end_time().time_since_epoch().count() << endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

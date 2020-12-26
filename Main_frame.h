//
// Created by shahrooz on 12/15/20.
//

#ifndef ENGLISH_TEACHER_MAIN_FRAME_H
#define ENGLISH_TEACHER_MAIN_FRAME_H

#include <wx/wx.h>

class Main_frame : public wxFrame{
public:
    Main_frame();

private:
    void set_structure();

    wxTextCtrl *video_address;
    wxTextCtrl *sub_address;
};


#endif //ENGLISH_TEACHER_MAIN_FRAME_H

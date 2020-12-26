//
// Created by shahrooz on 12/15/20.
//

#include "Application.h"
#include "Main_frame.h"

bool Application::OnInit(){
    Main_frame *main_frame = new Main_frame();
    main_frame->Show();
    return true;
}
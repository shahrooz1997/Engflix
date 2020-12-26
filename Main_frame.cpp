//
// Created by shahrooz on 12/15/20.
//

#include "Main_frame.h"

Main_frame::Main_frame() : wxFrame(nullptr, wxID_ANY, "English Learning", wxPoint(0, 0), wxSize(1000, 1000), wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP){
    wxMenu *menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, _T("&Open"));
    menuFile->Append(wxID_SAVE, _T("&Save"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, _T("&Quit"));
//    menuFile->Append(wxID_EXIT);
//    menuFile->AppendSeparator();
//    menuFile->Append(wxID_ABOUT);

    wxMenu *menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menu = new wxMenuBar();
    menu->Append(menuFile, "&File");
    menu->Append(menuHelp, "&Help");

    SetMenuBar(menu);
//    CreateStatusBar();
//    SetStatusText("Welcome to English Learning best ever platform!");
    Bind(wxEVT_MENU, [&](wxCommandEvent& event){wxMessageBox("This is a wxWidgets Hello World example",
                                             "About Hello World", wxOK | wxICON_INFORMATION);}, wxID_ABOUT);
    Bind(wxEVT_MENU, [&](wxCommandEvent& event){Close(true);}, wxID_EXIT);

    this->video_address = new wxTextCtrl(this, wxID_ANY, wxT("Enter the address of the video"), wxPoint(10, 10), wxSize(500, 30),  wxTE_AUTO_SCROLL);
    this->sub_address = new wxTextCtrl(this, wxID_ANY, wxT("Enter the address of sub video"), wxPoint(10, 10), wxSize(500, 30), wxTE_AUTO_SCROLL);

    Layout();
}

void Main_frame::set_structure(){

}
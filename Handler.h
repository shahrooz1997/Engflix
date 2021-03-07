//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_HANDLER_H
#define ENGLISH_TEACHER_HANDLER_H

#include "Util.h"
#include "Subtitle.h"
#include "VLC_interface.h"
#include <thread>
#include <iostream>
#include <ncurses.h>
#include "Interface.h"


enum Sub_state{
    start,
    end
};

class Handler final{
public:

    static Handler* get_instance();
    static Handler* get_instance(const string& subtitle_path);
    static void del_instance();

    void start();

private:
    static Handler* instance; // Singleton class
    static bool is_destroyed;

    explicit Handler(const std::string& subtitle_path);
    ~Handler() = default;

    int wait_until_subtitle(const One_subtitle& sub, Sub_state state, shared_ptr<bool> thread_cancel_p = nullptr);
    int wait_until_subtitle(const One_subtitle& sub, Sub_state state, const milliseconds& delay, shared_ptr<bool> thread_cancel_p = nullptr);

    void handle_one_subtitle(const One_subtitle& sub, bool seek_to_sub = false, shared_ptr<bool> thread_cancel_p = nullptr);

    Subtitle subtitles;
    unique_ptr<Interface> player;
};


#endif //ENGLISH_TEACHER_HANDLER_H

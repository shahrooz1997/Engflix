//
// Created by shahrooz on 10/29/20.
//

#include "Handler.h"

#undef OK

Handler* Handler::instance = nullptr;
bool Handler::is_destroyed = false;

Handler* Handler::get_instance(){
    assert(!is_destroyed);
    if(instance != nullptr){
        return instance;
    }
    assert(false);
}

Handler* Handler::get_instance(const string& subtitle_path){
    assert(!is_destroyed);
    if(instance != nullptr){
        return instance;
    }
    instance = new Handler(subtitle_path);
    instance->player.reset(new VLC_interface());
    return instance;
}

void Handler::del_instance(){
    delete get_instance();
    is_destroyed = true;
}

int Handler::wait_until_subtitle(const One_subtitle& sub, Sub_state state, shared_ptr<bool> thread_cancel_p){
    wait_until_subtitle(sub, state, milliseconds(0), thread_cancel_p);
}

int Handler::wait_until_subtitle(const One_subtitle& sub, Sub_state state, const milliseconds& delay, shared_ptr<bool> thread_cancel_p){
    auto last_tp = player->tell();
    while(!thread_cancel_p || !*thread_cancel_p){
        auto vlc_tp = player->tell();
        if(vlc_tp > last_tp + milliseconds(CHECK_LOOP_DELAY_MS + 10000)){
            DPRINTF(DEBUG_Handler, "%li > %li\n", vlc_tp.time_since_epoch().count(), (last_tp + milliseconds(CHECK_LOOP_DELAY_MS + 100)).time_since_epoch().count());
            return Status::MANUAL_SEEK_DETECTED;
        }
        if(state == Sub_state::start){
            if(vlc_tp > sub.get_start_time() + delay){
                return Status::OK;
            }
        }
        else if(state == Sub_state::end){
            if(vlc_tp > sub.get_end_time() + delay){
                return Status::OK;
            }
        }
        last_tp = vlc_tp;
        this_thread::sleep_for(milliseconds(CHECK_LOOP_DELAY_MS));
    }

    return Status::THREAD_CANCELED;
}

void Handler::handle_one_subtitle(const One_subtitle& sub, bool seek_to_sub, shared_ptr<bool> thread_cancel_p){
    int status = Status::OK;
    if(!seek_to_sub){
        status = wait_until_subtitle(sub, Sub_state::start, milliseconds(-1000), thread_cancel_p);
    }
    else{
        player->seek(sub.get_start_time() + milliseconds(-10));
    }
    if(status == Status::MANUAL_SEEK_DETECTED){
        DPRINTF(DEBUG_Handler, "AAA\n");
        assert(subtitles.set_subtitle_index(player->tell() + milliseconds(1)) == Status::OK);
        handle_one_subtitle(subtitles.get_subtitle(), true, thread_cancel_p);
        return;
    }
    if(thread_cancel_p && *thread_cancel_p){
        return;
    }
    cout << sub.get_text() << endl;
    status = wait_until_subtitle(sub, Sub_state::end, milliseconds(0), thread_cancel_p);
    if(status == Status::MANUAL_SEEK_DETECTED){
        DPRINTF(DEBUG_Handler, "bbbb\n");
        assert(subtitles.set_subtitle_index(player->tell() + milliseconds(1)) == Status::OK);
        handle_one_subtitle(subtitles.get_subtitle(), true, thread_cancel_p);
        return;
    }
    if(thread_cancel_p && *thread_cancel_p){
        return;
    }
    player->pause();
    player->seek(sub.get_end_time() - milliseconds(2)); // The delay is for keeping the subtitle on the Player
}

// Todo: resolve the screen problem in showing content for new line
// Todo: Make the application consistent with the player when it is waiting for input but the user seeks. For now, he must press q after the seeking
void Handler::start(){
    initscr();
    clear();
    noecho();
    cbreak();
    refresh();

    subtitles.reset();
    player->seek(time_point<steady_clock, seconds>(seconds(0)));

//    atomic_bool thread_cancel_first = bool;
//    handle_one_subtitle(thread_cancel_first, subtitles.get_subtitle());
    int in_key = 'd';
    while(!subtitles.is_finished()){
        shared_ptr<bool> thread_cancel_p(new bool);
        *thread_cancel_p = false;
        if(in_key == 'q'){
            player->play();
            assert(subtitles.set_subtitle_index(player->tell() + milliseconds(1)) == Status::OK);
            thread(&Handler::handle_one_subtitle, this, ref(subtitles.get_subtitle()), true, thread_cancel_p).detach();
        }
        else if(in_key == 'f'){
            player->play();
            thread(&Handler::handle_one_subtitle, this, ref(subtitles.get_subtitle()), true, thread_cancel_p).detach();
        }
        else if(in_key == 'd'){
            player->play();
            thread(&Handler::handle_one_subtitle, this, ref(subtitles.get_subtitle()), false, thread_cancel_p).detach();
//            handle_one_subtitle(subtitles.get_subtitle());
        }
        else if(in_key == 's'){
            player->play();
            subtitles.decrease_subtitle_index();
            thread(&Handler::handle_one_subtitle, this, ref(subtitles.get_subtitle()), true, thread_cancel_p).detach();
//            handle_one_subtitle(subtitles.get_subtitle(), true);
        }
        else if(in_key == 'a'){
            player->play();
            subtitles.decrease_subtitle_index();
            subtitles.decrease_subtitle_index();
            thread(&Handler::handle_one_subtitle, this, ref(subtitles.get_subtitle()), true, thread_cancel_p).detach();
//            handle_one_subtitle(subtitles.get_subtitle(), true);
        }
        else if(in_key == 'w'){
            player->play_pause();
        }
        else{
            cout << "Wrong input" << endl;
        }
        in_key = getch();
        if(in_key == 'q' || in_key == 'f' || in_key == 'd' || in_key == 'f' || in_key == 's' || in_key == 'a' || in_key == 'w'){
            *thread_cancel_p = true;
        }
    }

}

Handler::Handler(const string& subtitle_path) : subtitles(subtitle_path){
}

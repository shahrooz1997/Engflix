//
// Created by shahrooz on 10/29/20.
//

#include "Subtitle.h"
#include <algorithm>

Subtitle::Subtitle(const string& file_path) : current_subtitle_index(0){
    ifstream in(file_path, ios::in);
    assert(in.is_open());

    string one_sub_text;
    while(!in.eof()){
        string temp;
        getline(in, temp);
        // Standardization
        temp.erase(remove_if(temp.begin(), temp.end(), [](char c) {return !isprint(c);} ), temp.end());
//        if(temp[temp.size() - 1] == 0x0D)
//            temp.erase(temp.size() - 1, 1);
        if(temp.empty()){
            this->subtitles.emplace_back(one_sub_text);
            one_sub_text.clear();
            continue;
        }
        one_sub_text += temp;
        one_sub_text += "\n";
    }

    in.close();
}

void Subtitle::reset(){
    this->current_subtitle_index = 0;
}

int Subtitle::increase_subtitle_index(){
    if(is_finished()){
        return Status::END_OF_SUBTITLES;
    }
    this->current_subtitle_index++;
    return Status::OK;
}

int Subtitle::decrease_subtitle_index(){
    if(this->current_subtitle_index == 0){
        return Status::OK;
    }
    this->current_subtitle_index--;
    return Status::OK;
}

int Subtitle::set_subtitle_index(const uint& indx){
    if(indx > subtitles.size()){
        return Status::ID_TOO_BIG;
    }
    this->current_subtitle_index = indx;
    return Status::OK;
}

int Subtitle::set_subtitle_index(const time_point<steady_clock, milliseconds>& tp){
    int ret = Status::BAD_TIME_POINT;
    for(uint i = 0; i < subtitles.size(); i++){
//        DPRINTF(DEBUG_Subtitile, "%llu <= %llu\n", tp.time_since_epoch().count(), subtitles[i].get_end_time().time_since_epoch().count());
        if(tp < subtitles[i].get_end_time()){
            assert(set_subtitle_index(i) == Status::OK);
//            DPRINTF(DEBUG_Subtitile, "%llu\n", tp.time_since_epoch().count());
            ret = Status::OK;
            break;
        }
    }
    return ret;
}

const uint& Subtitle::get_subtitle_index() const{
    return this->current_subtitle_index;
}

bool Subtitle::is_finished() const{
    if(get_subtitle_index() >= subtitles.size()){
        return true;
    }
    return false;
}

const One_subtitle& Subtitle::get_current_subtitle(){
    return subtitles[this->get_subtitle_index()];
}

const One_subtitle& Subtitle::get_subtitle(){
    uint t_indx = this->get_subtitle_index();
    this->increase_subtitle_index();
    return subtitles[t_indx];
}

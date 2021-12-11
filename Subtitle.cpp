//
// Created by shahrooz on 10/29/20.
//

#include "Subtitle.h"
#include <algorithm>
#include <iostream>

using namespace std;

Subtitle::Subtitle(const string& file_path) : current_subtitle_index(0){
    ifstream in_file(file_path, ios::in);
    if(!in_file.is_open()){
        cerr << "Cannot open the file \"" << file_path << "\"" << endl;
        exit(2);
    }

    string one_sub_text;
    while(!in_file.eof()){
        string temp;
        getline(in_file, temp);
        // Standardization
        temp.erase(remove_if(temp.begin(), temp.end(), [](char c) {return !isprint(c);} ), temp.end());

        if(temp.empty()){ // subtitle ends
            if(one_sub_text.empty()){
                continue;
            }
            this->subtitles.emplace_back(one_sub_text, this);
            one_sub_text.clear();
            continue;
        }
        one_sub_text += temp;
        one_sub_text += "\n";
    }

    in_file.close();
}

void Subtitle::reset(){
    this->current_subtitle_index = 0;
}

int Subtitle::increase_subtitle_index(){
    if(is_finished()){
        return EngFlix_Status::END_OF_SUBTITLES;
    }
    this->current_subtitle_index++;
    return EngFlix_Status::OK;
}

int Subtitle::decrease_subtitle_index(){
    if(this->current_subtitle_index == 0){
        return EngFlix_Status::OK;
    }
    this->current_subtitle_index--;
    return EngFlix_Status::OK;
}

int Subtitle::set_subtitle_index(const uint& indx){
    if(indx > subtitles.size()){
        return EngFlix_Status::ID_TOO_BIG;
    }
    this->current_subtitle_index = indx;
    return EngFlix_Status::OK;
}

int Subtitle::set_subtitle_index(const time_point<steady_clock, milliseconds>& tp){
    int ret = EngFlix_Status::BAD_TIME_POINT;
    for(uint i = 0; i < subtitles.size(); i++){
//        DPRINTF(DEBUG_Subtitile, "%llu <= %llu\n", tp.time_since_epoch().count(), subtitles[i].get_end_time().time_since_epoch().count());
        if(tp < subtitles[i].get_end_time()){
            assert(set_subtitle_index(i) == EngFlix_Status::OK);
//            DPRINTF(DEBUG_Subtitile, "%llu\n", tp.time_since_epoch().count());
            ret = EngFlix_Status::OK;
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
const vector<One_subtitle> &Subtitle::get_subtitles() const {
  return subtitles;
}



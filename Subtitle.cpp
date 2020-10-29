//
// Created by shahrooz on 10/29/20.
//

#include "Subtitle.h"

Subtitle::Subtitle(const string& file_path) : current_subtitle_index(0){
    ifstream in(file_path, ios::in);
    assert(in.is_open());

    string one_sub_text;
    while(!in.eof()){
        string temp;
        getline(in, temp);
        // Standardization
        if(temp[temp.size() - 1] == 0x0D)
            temp.erase(temp.size() - 1, 1);
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

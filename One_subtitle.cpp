//
// Created by shahrooz on 10/27/20.
//

#include "One_subtitle.h"

One_subtitle::One_subtitle(const string& in){
    vector<string> lines = split_string_the_first_two_lines(in);
    this->id = stoul(lines[0]);
    get_start_time_end_time_from_string(lines[1], this->start_time, this->end_time);
    this->text = lines[2];
}

uint One_subtitle::get_id() const{
    return id;
}

void One_subtitle::set_id(uint id){
    One_subtitle::id = id;
}

const time_point<steady_clock, milliseconds>& One_subtitle::get_start_time() const{
    return start_time;
}

void One_subtitle::set_start_time(const time_point<steady_clock, milliseconds>& start_time){
    One_subtitle::start_time = start_time;
}

const time_point<steady_clock, milliseconds>& One_subtitle::get_end_time() const{
    return end_time;
}

void One_subtitle::set_end_time(const time_point<steady_clock, milliseconds>& end_time){
    One_subtitle::end_time = end_time;
}

const string& One_subtitle::get_text() const{
    return text;
}

void One_subtitle::set_text(const string& text){
    One_subtitle::text = text;
}

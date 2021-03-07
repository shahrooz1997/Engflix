//
// Created by shahrooz on 10/27/20.
//

#include "One_subtitle.h"
#include <algorithm>
#include <iostream>

using namespace std;

One_subtitle::One_subtitle(const string& in){
    DPRINTF(DEBUG_Subtitile, "The input string is %s\n", in.c_str());
    vector<string> lines = split_subtitle_string(in);
    if(lines.size() != 3){
        cerr << "Wrong subtitle format:\n" << "\"" << in << "\"" << endl;
        exit(2);
    }

    this->id = stoul(lines[0]);
    set_start_time_end_time_from_string(lines[1], this->start_time, this->end_time);
    this->text = lines[2];
}

uint One_subtitle::get_id() const{
    return id;
}

const time_point<steady_clock, milliseconds>& One_subtitle::get_start_time() const{
    return start_time;
}

const time_point<steady_clock, milliseconds>& One_subtitle::get_end_time() const{
    return end_time;
}

const string& One_subtitle::get_text() const{
    return text;
}

std::vector<std::string> One_subtitle::split_subtitle_string(const std::string& in){
    vector<string> ret;
    size_t perv_pos = 0;
    size_t i = 0;
    while(perv_pos < in.size()){
        size_t curr_pos = in.find("\n", perv_pos);
        ret.push_back(in.substr(perv_pos, curr_pos - perv_pos));
        perv_pos = curr_pos + 1;
        i++;
        if(i == 2){
            break;
        }
    }
    if(in[in.size() - 1] == '\n'){
        ret.push_back(in.substr(perv_pos, in.size() - perv_pos - 1));
    }
    else{
        ret.push_back(in.substr(perv_pos));
    }

    // Standardization
    size_t last_pos = 0;
    while(true){
        last_pos = ret[2].find("\n", last_pos);
        if(last_pos == -1){
            break;
        }
        ret[2].replace(last_pos, 1, " ");
    }
    for(auto& line: ret){
        line.erase(remove_if(line.begin(), line.end(), [](char c) {return !isprint(c);} ), line.end());
    }

    return ret;
}

std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> One_subtitle::stotime(const std::string& in){
    time_point<steady_clock, milliseconds> ret;
    hours hr(stoull(in.substr(0,2)));
    minutes min(stoull(in.substr(3,2)));
    seconds sec(stoull(in.substr(6,2)));
    milliseconds ms(stoull(in.substr(9,3)));
    ret += hr;
    ret += min;
    ret += sec;
    ret += ms;
    return ret;
}

int One_subtitle::set_start_time_end_time_from_string(const std::string& in,
                                                      std::chrono::time_point<steady_clock, milliseconds>& start_time,
                                                      std::chrono::time_point<steady_clock, milliseconds>& end_time){
    size_t pos = in.find(" --> ");
    assert(pos != string::npos);
    start_time = stotime(in.substr(0, pos));
    end_time = stotime(in.substr(pos + 5));
    return Status::OK;
}
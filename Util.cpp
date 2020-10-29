//
// Created by shahrooz on 10/29/20.
//

#include "Util.h"

using std::string;
using std::vector;
using namespace std::chrono;

vector<string> split_string_the_first_two_lines(const string& in){
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
    ret.push_back(in.substr(perv_pos));
    return ret;
}

time_point<steady_clock, milliseconds> stotime(const string& in){ // 00:00:04,263 -> 4263
    time_point<steady_clock, milliseconds> ret;
    hours hr(stoull(in.substr(0,2)));
    minutes min(stoull(in.substr(3,2)));
    seconds sec(stoull(in.substr(6,2)));
    milliseconds ms(stoull(in.substr(9,3)));
    ret += hr;
    ret + min;
    ret += sec;
    ret += ms;
    return ret;
}

template <typename T, typename U>
int get_start_time_end_time_from_string(const string& in, time_point<T, U>& start_time, time_point<T, U>& end_time){
    size_t pos = in.find(" --> ");
    assert(pos != string::npos);
    start_time = stotime(in.substr(0, pos));
    end_time = stotime(in.substr(pos + 5));
    return Status::OK;
}
template int get_start_time_end_time_from_string(const std::string& in,
                                                 std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>& start_time,
                                                 std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>& end_time);
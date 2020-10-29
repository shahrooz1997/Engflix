//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_UTIL_H
#define ENGLISH_TEACHER_UTIL_H

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <assert.h>

enum Status{
    OK = 0,
    END_OF_SUBTITLES = -1,
    ID_TOO_BIG = -2
};

std::vector<std::string> split_string_the_first_two_lines(const std::string& in);

std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> stotime(const std::string& in);

template <typename T, typename U>
int get_start_time_end_time_from_string(const std::string& in, std::chrono::time_point<T, U>& start_time,
                                        std::chrono::time_point<T, U>& end_time);


#endif //ENGLISH_TEACHER_UTIL_H

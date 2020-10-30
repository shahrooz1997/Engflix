//
// Created by shahrooz on 10/27/20.
//

#ifndef ENGLISH_TEACHER_ONE_SUBTITLE_H
#define ENGLISH_TEACHER_ONE_SUBTITLE_H

#include "Util.h"

// Note: in the case that you need a default constructor for this class you can uncomment the commented codes

using namespace std;
using namespace std::chrono;

typedef uint32_t uint;

class One_subtitle{
public:
    explicit One_subtitle(const string& in);
    virtual ~One_subtitle() = default;

    uint get_id() const;
//    void set_id(uint id);

    const time_point<steady_clock, milliseconds>& get_start_time() const;
//    void set_start_time(const time_point<steady_clock, milliseconds>& start_time);

    const time_point<steady_clock, milliseconds>& get_end_time() const;
//    void set_end_time(const time_point<steady_clock, milliseconds>& end_time);

    const string& get_text() const;
//    void set_text(const string& text);

private:
    uint id;
    time_point<steady_clock, milliseconds> start_time;
    time_point<steady_clock, milliseconds> end_time;
    string text;

    static std::vector<std::string> split_string_the_first_two_lines(const std::string& in);
    static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> stotime(const std::string& in);
    static int get_start_time_end_time_from_string(const std::string& in,
                                                   std::chrono::time_point<steady_clock, milliseconds>& start_time,
                                                   std::chrono::time_point<steady_clock, milliseconds>& end_time);

};


#endif //ENGLISH_TEACHER_ONE_SUBTITLE_H

//
// Created by shahrooz on 10/27/20.
//

#ifndef ENGLISH_TEACHER_ONE_SUBTITLE_H
#define ENGLISH_TEACHER_ONE_SUBTITLE_H

#include "Util.h"

using namespace std;
using namespace std::chrono;

typedef uint32_t uint;

class One_subtitle{
public:
    explicit One_subtitle(const string& in);
    virtual ~One_subtitle() = default;

    uint get_id() const;
    void set_id(uint id);

    const time_point<steady_clock, milliseconds>& get_start_time() const;
    void set_start_time(const time_point<steady_clock, milliseconds>& start_time);

    const time_point<steady_clock, milliseconds>& get_end_time() const;
    void set_end_time(const time_point<steady_clock, milliseconds>& end_time);

    const string& get_text() const;
    void set_text(const string& text);

private:
    uint id;
    time_point<steady_clock, milliseconds> start_time;
    time_point<steady_clock, milliseconds> end_time;
    string text;
};


#endif //ENGLISH_TEACHER_ONE_SUBTITLE_H

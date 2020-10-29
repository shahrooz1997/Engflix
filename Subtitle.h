//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_SUBTITLE_H
#define ENGLISH_TEACHER_SUBTITLE_H

#include "Util.h"
#include "One_subtitle.h"
#include <fstream>

using namespace std;

class Subtitle{
public:
    explicit Subtitle(const string& file_path);
    virtual ~Subtitle() = default;

    void reset();
    int increase_subtitle_index();
    int decrease_subtitle_index();
    int set_subtitle_index(const uint& indx);
    const uint& get_subtitle_index() const;
    bool is_finished() const;

    const One_subtitle& get_current_subtitle();
    const One_subtitle& get_subtitle();

private:
    vector<One_subtitle> subtitles;
    uint current_subtitle_index;
};


#endif //ENGLISH_TEACHER_SUBTITLE_H

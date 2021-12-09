//
// Created by shahrooz on 10/27/20.
//

#ifndef ENGLISH_TEACHER_ONE_SUBTITLE_H
#define ENGLISH_TEACHER_ONE_SUBTITLE_H

#include "Util.h"

class Subtitle;

using namespace std;
using namespace std::chrono;

typedef uint32_t uint;

class One_subtitle {
 public:
  explicit One_subtitle(const string &in, Subtitle *parent);
  virtual ~One_subtitle() = default;

  uint get_id() const;
  const time_point<steady_clock, milliseconds> &get_start_time() const;
  const time_point<steady_clock, milliseconds> &get_end_time() const;
  const string &get_text() const;

  const One_subtitle *get_next() const;
  const One_subtitle *get_prev() const;

 private:
  uint id;
  time_point<steady_clock, milliseconds> start_time;
  time_point<steady_clock, milliseconds> end_time;
  string text;
  Subtitle *parent;

  static std::vector<std::string> split_subtitle_string(const std::string &in);
  static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> stotime(const std::string &in);
  static int set_start_time_end_time_from_string(const std::string &in,
                                                 std::chrono::time_point<steady_clock, milliseconds> &start_time,
                                                 std::chrono::time_point<steady_clock, milliseconds> &end_time);

};

#endif //ENGLISH_TEACHER_ONE_SUBTITLE_H

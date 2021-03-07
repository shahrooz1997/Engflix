//
// Created by shahrooz on 3/7/21.
//

#ifndef ENGLISH_TEACHER_INTERFACE_H
#define ENGLISH_TEACHER_INTERFACE_H

enum State{
    Playing,
    Paused,
    Stopped
};

class Interface{
public:
    virtual ~Interface() = default;
    virtual void close() = 0;
    virtual void play_pause() = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual State get_state() = 0;
    virtual void seek(const std::chrono::milliseconds& dur) = 0;
    virtual void seek(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>& tp) = 0;
    virtual std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> tell() = 0;
};

#endif //ENGLISH_TEACHER_INTERFACE_H

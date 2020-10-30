//
// Created by shahrooz on 10/29/20.
//

#ifndef ENGLISH_TEACHER_UTIL_H
#define ENGLISH_TEACHER_UTIL_H

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>

#define CHECK_LOOP_DELAY_MS 10

extern bool DEBUG_Subtitile;
extern bool DEBUG_VLC_interface;
extern bool DEBUG_Handler;

#define DPRINTF(flag, fmt, ...) \
    do{ \
        if(flag) \
            fprintf(stdout, "Time %10li - Thread: %lu : [%s][%s]%d: " fmt, time(nullptr), pthread_self(), __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout); \
    } while(0)

enum Status{
    OK = 0,
    END_OF_SUBTITLES = -1,
    ID_TOO_BIG = -2,
    BAD_TIME_POINT = -3,
    THREAD_CANCELED = -4,
    MANUAL_SEEK_DETECTED = -5
};


#endif //ENGLISH_TEACHER_UTIL_H

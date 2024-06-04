#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#include <sys/time.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "Singleton.h"

class Time {
   public:
    static long GetNowS() {
        return std::time(nullptr);
    }

    static long GetNowMs() {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

    static long GetNowMicroS() {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000000 + tv.tv_usec;
    }

    static long long GetNowNs() {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return static_cast<long long>(ts.tv_sec) * 1000000000 + ts.tv_nsec;
    }

    char* DateTime() {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto current_time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(epoch).count() % 1000000;

        std::tm* currentTime = std::localtime(&current_time);

        char* formattedDateTime = new char[25];  // Allocate memory for the formatted date and time
        std::sprintf(formattedDateTime,
                     "%04d-%02d-%02d %02d:%02d:%02d:%06ld",
                     currentTime->tm_year + 1900,
                     currentTime->tm_mon + 1,
                     currentTime->tm_mday,
                     currentTime->tm_hour,
                     currentTime->tm_min,
                     currentTime->tm_sec,
                     ms);

        return formattedDateTime;
    }
};

#define TimeUtil Singleton<Time>::Instance()

#endif  // UTIL_TIME_H
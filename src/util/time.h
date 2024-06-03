#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#include <sys/time.h>

#include <ctime>
#include <iomanip>
#include <iostream>

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

    static std::string GetNowDate() {
        std::time_t now = std::time(nullptr);
        std::tm *now_tm = std::localtime(&now);

        std::stringstream ss;
        ss << std::put_time(now_tm, "%Y-%m-%d");
        return ss.str();
    }
};

#endif  // UTIL_TIME_H
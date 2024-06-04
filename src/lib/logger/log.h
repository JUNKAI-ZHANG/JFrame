#ifndef LOG_LOG_H
#define LOG_LOG_H

#include <iostream>

class Log {
   public:
    template <typename... Args>
    void Debug(Args... args) {
        std::cout << "[" << __DATE__ << " " << __TIME__ << "] [Debug] [" << (std::cout << ... << args) << "]" << std::endl;
    }

    template <typename... Args>
    void Info(Args... args) {
        std::cout << "[" << __DATE__ << " " << __TIME__ << "] [Info] [";
        (std::cout << ... << args) << "]" << std::endl;
    }

    template <typename... Args>
    void Warn(Args... args) {
        std::cout << "[" << __DATE__ << " " << __TIME__ << "] [Warn] [" << (std::cout << ... << args) << "]" << std::endl;
    }

    template <typename... Args>
    void Error(Args... args) {
        std::cout << "[" << __DATE__ << " " << __TIME__ << "] [Error] [" << (std::cout << ... << args) << "]" << std::endl;
    }

    template <typename... Args>
    void Fatal(Args... args) {
        std::cout << "[" << __DATE__ << " " << __TIME__ << "] [Fatal] [" << (std::cout << ... << args) << "]" << std::endl;
    }
};

#endif  // LOG_LOG_H
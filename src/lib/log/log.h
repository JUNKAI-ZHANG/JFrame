#ifndef LOG_LOG_H
#define LOG_LOG_H

#include <stdio.h>

#include <string>

#include "lib/util/Singleton.h"
#include "lib/util/Time.h"

class Logger {
   public:
    explicit Logger() {
    }

    template <typename... Args>
    inline void Format(const char* cLogLevel, Args... kArgs) {
        printf("[%s][game][%s][%s:%d][%s]\n",
               TimeUtil.DateTime(),
               cLogLevel,
               __FILE__,
               __LINE__,
               ArgsExpansion(kArgs...).c_str());
    }

    // 递归展开参数列表并转换为字符串
    template <typename T>
    void ArgsExpansion(std::ostringstream& kOStringStream, const T& kArg) {
        kOStringStream << kArg;
    }

    template <typename T, typename... Args>
    void ArgsExpansion(std::ostringstream& kOStringStream, const T& kArg, const Args&... kArgs) {
        kOStringStream << kArg << " ";
        ArgsExpansion(kOStringStream, kArgs...);
    }

    // 将参数列表 Args... args 转为字符串
    template <typename... Args>
    std::string ArgsExpansion(Args... kArgs) {
        std::ostringstream kOStringStream;
        ArgsExpansion(kOStringStream, kArgs...);
        return kOStringStream.str();
    }
};

#define Log Singleton<Logger>::Instance()
#define LogDebug(...) printf("[%s][game][Debug][%s:%d][%s]\n", TimeUtil.DateTime(), __FILE__, __LINE__, Log.ArgsExpansion(__VA_ARGS__).c_str());
#define LogInfo(...) printf("[%s][game][Info][%s:%d][%s]\n", TimeUtil.DateTime(), __FILE__, __LINE__, Log.ArgsExpansion(__VA_ARGS__).c_str());
#define LogWarn(...) printf("[%s][game][Warn][%s:%d][%s]\n", TimeUtil.DateTime(), __FILE__, __LINE__, Log.ArgsExpansion(__VA_ARGS__).c_str());
#define LogError(...) printf("[%s][game][Error][%s:%d][%s]\n", TimeUtil.DateTime(), __FILE__, __LINE__, Log.ArgsExpansion(__VA_ARGS__).c_str());
#define LogFatal(...) printf("[%s][game][Fatal][%s:%d][%s]\n", TimeUtil.DateTime(), __FILE__, __LINE__, Log.ArgsExpansion(__VA_ARGS__).c_str());

#endif  // LOG_LOG_H
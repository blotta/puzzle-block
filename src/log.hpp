#ifndef LOG_HPP
#define LOG_HPP

#include <SDL2/SDL.h>
#include <cstdarg>
#include <string>

enum class LogLevel
{
    Info,
    Warn,
    Error,
    Debug
};

class Log
{
  public:
    static void log(LogLevel level, const char* fmt, ...);

    static void info(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void error(const char* fmt, ...);
    static void debug(const char* fmt, ...);
};

#endif
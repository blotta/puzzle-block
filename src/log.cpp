// Logger.cpp
#include "log.hpp"

void Log::log(LogLevel level, const char* fmt, ...)
{
    int sdlLevel;
    switch (level)
    {
    case LogLevel::Info:
        sdlLevel = SDL_LOG_PRIORITY_INFO;
        break;
    case LogLevel::Warn:
        sdlLevel = SDL_LOG_PRIORITY_WARN;
        break;
    case LogLevel::Error:
        sdlLevel = SDL_LOG_PRIORITY_ERROR;
        break;
    case LogLevel::Debug:
        sdlLevel = SDL_LOG_PRIORITY_DEBUG;
        break;
    default:
        sdlLevel = SDL_LOG_PRIORITY_INFO;
        break;
    }

    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, (SDL_LogPriority)sdlLevel, fmt, args);
    va_end(args);
}

void Log::info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, args);
    va_end(args);
}

void Log::warn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, fmt, args);
    va_end(args);
}

void Log::error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, args);
    va_end(args);
}

void Log::debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, fmt, args);
    va_end(args);
}

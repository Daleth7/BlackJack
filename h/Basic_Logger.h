#ifndef BASIC_LOGGER_H
#define BASIC_LOGGER_H

#include <ostream>
#include <string>

struct Basic_Logger{
    template <typename message>
        static void log_helper(message);
    template <typename message0, typename... messages>
        static void log_helper(message0, messages...);
    template <typename... messages>
        static void log(messages...);
    static std::ostream* output;
    static std::string filepath;
    static std::ios_base::openmode flags;
    Basic_Logger() = delete;
};

#include "Basic_Logger.inl"

#ifdef LOG_MODE
    #define log(...) Basic_Logger::log(__func__, "(): ", __VA_ARGS__)
#else
    #define log(...) while(0)
#endif

#endif
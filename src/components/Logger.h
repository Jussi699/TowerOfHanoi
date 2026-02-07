#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <chrono>

namespace Logger
{
    enum class Level { Debug, Info, Warning, Error, Test };

    void Init(const std::string& path = "Logs/app.log");
    void Log(Level level, const std::string& message);
    inline std::string LevelToString(Level l) {
        switch (l) {
        case Level::Debug: return "DEBUG";
        case Level::Info: return "INFO";
        case Level::Warning: return "WARN";
        case Level::Error: return "ERROR";
        case Level::Test: return "TEST";
        default: return "UNK";
        }
    }
}
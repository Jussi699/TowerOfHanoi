#include "Logger.h"
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <iostream>

namespace Logger
{
    static std::ofstream s_logFile;
    static std::mutex s_mutex;
    static bool s_initialized = false;
    static std::string s_logPath = "Logs/app.log";
    static bool s_newSession = true;

    void Init(const std::string& path)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (s_initialized && path == s_logPath) return;

        s_logPath = path;
        try {
            std::filesystem::create_directories(std::filesystem::path(s_logPath).parent_path());
 
            s_logFile.open(s_logPath, std::ios::trunc);
            s_initialized = s_logFile.is_open();
            if (!s_initialized) {
                std::cerr << "Logger: failed to open log file: " << s_logPath << std::endl;
            }
        }
        catch (const std::exception& ex) {
            s_initialized = false;
            std::cerr << "Logger Init exception: " << ex.what() << std::endl;
        }
    }

    void Log(Level level, const std::string& message)
    {
        if (!s_initialized) Init(s_logPath);

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" << LevelToString(level) << "] " << message << "\n";

        std::string out = oss.str();

        {
            std::lock_guard<std::mutex> lock(s_mutex);
            if (s_initialized) {
                s_logFile << out;
                s_logFile.flush();
            }
            std::cerr << out;
        }
    }
}
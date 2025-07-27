// logger.h
// Thread-safe logger framework for printing file, line, thread id, log level, and message.
// Logs are written to both stdout and a log file.
// Usage: LOG(INFO, "Message");
#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <mutex>

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

inline const char* LogLevelToString(LogLevel level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

class Logger {
public:
    static Logger& Instance() {
        static Logger instance;
        return instance;
    }

    void Log(LogLevel level, const char* file, int line, const std::string& message) {
        std::ostringstream oss;
        oss << "[" << LogLevelToString(level) << "] "
            << file << ":" << line << " [TID: "
            << std::this_thread::get_id() << "] " << message << std::endl;
        std::lock_guard<std::mutex> lock(mutex_);
        // std::cout << oss.str();
        logfile_ << oss.str();
        logfile_.flush();
    }

private:
    Logger() : logfile_("project_kafka_cpp.log", std::ios::app) {}
    ~Logger() { logfile_.close(); }
    std::ofstream logfile_;
    std::mutex mutex_;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#define LOG(level, message) \
    Logger::Instance().Log(level, __FILE__, __LINE__, (std::ostringstream() << message).str())// logger.h

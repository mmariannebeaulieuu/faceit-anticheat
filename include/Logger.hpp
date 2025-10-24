#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <string_view>

class Logger {
public:
    static Logger& instance();

    void info(std::string_view message);
    void info(std::wstring_view message);

    void warn(std::string_view message);
    void warn(std::wstring_view message);

    void error(std::string_view message);
    void error(std::wstring_view message);

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void write(std::string_view level, std::string_view message);
    static std::string toUtf8(std::wstring_view message);

    std::ofstream file_;
    std::mutex mutex_;
};

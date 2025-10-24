#include "Logger.hpp"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <codecvt>
#include <locale>
#endif

namespace {
std::string timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto timeT = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &timeT);
#else
    localtime_r(&timeT, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    try {
        const auto logDir = std::filesystem::current_path() / "logs";
        std::filesystem::create_directories(logDir);
        file_.open(logDir / "agent.log", std::ios::app);
    } catch (const std::exception& ex) {
        std::cerr << "[LOGGER] failed to initialize log file: " << ex.what() << std::endl;
    }
}

Logger::~Logger() {
    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }
}

void Logger::info(std::string_view message) {
    write("INFO", message);
}

void Logger::info(std::wstring_view message) {
    info(toUtf8(message));
}

void Logger::warn(std::string_view message) {
    write("WARN", message);
}

void Logger::warn(std::wstring_view message) {
    warn(toUtf8(message));
}

void Logger::error(std::string_view message) {
    write("ERROR", message);
}

void Logger::error(std::wstring_view message) {
    error(toUtf8(message));
}

void Logger::write(std::string_view level, std::string_view message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ostringstream formatted;
    formatted << timestamp() << ' ' << '[' << level << "] " << message;
    const auto line = formatted.str();

    std::cout << line << std::endl;
    if (file_.is_open()) {
        file_ << line << std::endl;
    }
}

std::string Logger::toUtf8(std::wstring_view message) {
    if (message.empty()) {
        return {};
    }
#ifdef _WIN32
    int required = WideCharToMultiByte(
        CP_UTF8,
        0,
        message.data(),
        static_cast<int>(message.size()),
        nullptr,
        0,
        nullptr,
        nullptr);
    if (required <= 0) {
        return {};
    }
    std::string buffer(required, '\0');
    int written = WideCharToMultiByte(
        CP_UTF8,
        0,
        message.data(),
        static_cast<int>(message.size()),
        buffer.data(),
        required,
        nullptr,
        nullptr);
    if (written <= 0) {
        return {};
    }
    return buffer;
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(message.data(), message.data() + message.size());
#endif
}

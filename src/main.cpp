#include "Logger.hpp"
#include "ProcessEnumerator.hpp"

#include <sstream>

int wmain() {
    Logger::instance().info(L"CS:GO Anti-Cheat Prototype - Phase 1 initialization");

    ProcessEnumerator enumerator;
    const auto processes = enumerator.enumerate();

    for (const auto& process : processes) {
        std::wstringstream builder;
        builder << L"PID=" << process.pid << L" Name=" << process.name;
        if (!process.imagePath.empty()) {
            builder << L" Path=" << process.imagePath;
        }

        if (process.isWhitelisted) {
            builder << L" [WHITELISTED]";
            Logger::instance().info(builder.str());
        } else if (!process.accessible) {
            builder << L" [ACCESS LIMITED]";
            Logger::instance().warn(builder.str());
        } else {
            builder << L" [REVIEW]";
            Logger::instance().warn(builder.str());
        }
    }

    std::wstringstream summary;
    summary << L"Process enumeration complete. Total entries: " << processes.size();
    Logger::instance().info(summary.str());

    return 0;
}

#include "ProcessEnumerator.hpp"

#include "Logger.hpp"

#include <algorithm>
#include <cwctype>
#include <iterator>
#include <utility>
#include <vector>

#include <TlHelp32.h>

namespace {
std::wstring toLower(std::wstring_view value) {
    std::wstring lowered(value.begin(), value.end());
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(std::towlower(ch));
    });
    return lowered;
}
}

ProcessEnumerator::ProcessEnumerator() {
    const std::vector<std::wstring> defaults = {
        L"csrss.exe",
        L"explorer.exe",
        L"services.exe",
        L"winlogon.exe",
        L"svchost.exe",
        L"lsass.exe",
        L"smss.exe",
        L"System",
        L"System Idle Process"
    };

    whitelist_.reserve(defaults.size());
    for (const auto& name : defaults) {
        whitelist_.push_back(toLower(name));
    }
}

std::vector<ProcessInfo> ProcessEnumerator::enumerate() const {
    std::vector<ProcessInfo> results;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        Logger::instance().error("Failed to create process snapshot.");
        return results;
    }

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    if (!Process32FirstW(snapshot, &entry)) {
        Logger::instance().error("Failed to retrieve first process entry.");
        CloseHandle(snapshot);
        return results;
    }

    do {
        ProcessInfo info{};
        info.pid = entry.th32ProcessID;
        info.name.assign(entry.szExeFile);
        info.isWhitelisted = isWhitelistedName(info.name);
        info.accessible = false;

        HANDLE processHandle = OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION,
            FALSE,
            entry.th32ProcessID);

        if (processHandle != nullptr) {
            wchar_t buffer[MAX_PATH] = {0};
            DWORD size = static_cast<DWORD>(std::size(buffer));
            if (QueryFullProcessImageNameW(processHandle, 0, buffer, &size) != 0) {
                info.imagePath.assign(buffer, buffer + size);
            }
            info.accessible = true;
            CloseHandle(processHandle);
        }

        results.push_back(std::move(info));

    } while (Process32NextW(snapshot, &entry));

    CloseHandle(snapshot);
    return results;
}

bool ProcessEnumerator::isWhitelistedName(std::wstring_view name) const {
    const std::wstring normalized = normalizeName(name);
    return std::find(whitelist_.begin(), whitelist_.end(), normalized) != whitelist_.end();
}

std::wstring ProcessEnumerator::normalizeName(std::wstring_view name) {
    return toLower(name);
}

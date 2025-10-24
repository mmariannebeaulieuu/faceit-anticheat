#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <Windows.h>

struct ProcessInfo {
    DWORD pid;
    std::wstring name;
    std::wstring imagePath;
    bool isWhitelisted;
    bool accessible;
};

class ProcessEnumerator {
public:
    ProcessEnumerator();

    [[nodiscard]] std::vector<ProcessInfo> enumerate() const;

private:
    bool isWhitelistedName(std::wstring_view name) const;
    static std::wstring normalizeName(std::wstring_view name);

    std::vector<std::wstring> whitelist_;
};

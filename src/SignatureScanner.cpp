#include "SignatureScanner.hpp"

#include "Logger.hpp"

#include <TlHelp32.h>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>

namespace {
constexpr size_t kMaxModuleReadSize = 16u * 1024u * 1024u; // 16 MiB cap per module
}

SignatureScanner::SignatureScanner() {
    signatures_.push_back(Signature{
        .name = L"NOP sled (4-byte)",
        .pattern = {0x90, 0x90, 0x90, 0x90},
        .mask = {true, true, true, true},
    });

    signatures_.push_back(Signature{
        .name = L"Unconditional jump patch",
        .pattern = {0xE9, 0x00, 0x00, 0x00, 0x00},
        .mask = {true, false, false, false, false},
    });
}

std::vector<SignatureHit> SignatureScanner::scan(const ProcessInfo& process) const {
    std::vector<SignatureHit> hits;
    if (signatures_.empty()) {
        return hits;
    }

    HANDLE processHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        process.pid);

    if (processHandle == nullptr) {
        const DWORD lastError = GetLastError();
        std::wstringstream ss;
        ss << L"Signature scan skipped for PID=" << process.pid << L" (" << process.name << L")"
           << L" error=0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
           << lastError;
        Logger::instance().warn(ss.str());
        return hits;
    }

    HANDLE moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process.pid);
    if (moduleSnapshot == INVALID_HANDLE_VALUE) {
        std::wstringstream ss;
        ss << L"Unable to create module snapshot for PID=" << process.pid;
        Logger::instance().warn(ss.str());
        CloseHandle(processHandle);
        return hits;
    }

    MODULEENTRY32W moduleEntry{};
    moduleEntry.dwSize = sizeof(moduleEntry);

    if (!Module32FirstW(moduleSnapshot, &moduleEntry)) {
        std::wstringstream ss;
        ss << L"Module enumeration failed for PID=" << process.pid;
        Logger::instance().warn(ss.str());
        CloseHandle(moduleSnapshot);
        CloseHandle(processHandle);
        return hits;
    }

    do {
        if (moduleEntry.modBaseSize == 0) {
            continue;
        }

        const size_t regionSize = static_cast<size_t>(moduleEntry.modBaseSize);
        if (regionSize > kMaxModuleReadSize) {
            std::wstringstream ss;
            ss << L"Skipping large module '" << moduleEntry.szModule << L"' (" << regionSize << L" bytes)";
            Logger::instance().info(ss.str());
            continue;
        }

        std::vector<uint8_t> buffer(regionSize);
        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(
                processHandle,
                moduleEntry.modBaseAddr,
                buffer.data(),
                buffer.size(),
                &bytesRead)) {
            continue;
        }

        buffer.resize(bytesRead);
        for (const auto& signature : signatures_) {
            if (signature.pattern.size() != signature.mask.size()) {
                continue;
            }
            if (signature.pattern.size() > buffer.size()) {
                continue;
            }

            for (size_t i = 0; i + signature.pattern.size() <= buffer.size(); ++i) {
                if (!matchAt(buffer, i, signature)) {
                    continue;
                }

                hits.push_back(SignatureHit{
                    .signatureName = signature.name,
                    .moduleName = std::wstring(moduleEntry.szModule),
                    .address = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr) + i,
                });
            }
        }
    } while (Module32NextW(moduleSnapshot, &moduleEntry));

    CloseHandle(moduleSnapshot);
    CloseHandle(processHandle);
    return hits;
}

bool SignatureScanner::matchAt(const std::vector<uint8_t>& buffer, size_t index, const Signature& signature) const {
    for (size_t i = 0; i < signature.pattern.size(); ++i) {
        if (!signature.mask[i]) {
            continue;
        }
        if (buffer[index + i] != signature.pattern[i]) {
            return false;
        }
    }
    return true;
}

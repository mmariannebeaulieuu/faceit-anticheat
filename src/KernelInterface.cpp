#include "KernelInterface.hpp"

#include "Logger.hpp"

#include <iomanip>
#include <sstream>
#include <winioctl.h>

KernelInterface::KernelInterface()
    : handle_(INVALID_HANDLE_VALUE) {}

KernelInterface::~KernelInterface() {
    disconnect();
}

bool KernelInterface::connect() {
    if (isConnected()) {
        return true;
    }

    handle_ = CreateFileW(
        AC_USER_DEVICE_PATH,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (handle_ == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        std::wstringstream ss;
        ss << L"Kernel driver connection failed error=0x"
           << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
           << static_cast<unsigned long>(error);
        Logger::instance().warn(ss.str());
        return false;
    }

    return true;
}

void KernelInterface::disconnect() {
    if (handle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
    }
}

bool KernelInterface::isConnected() const {
    return handle_ != INVALID_HANDLE_VALUE;
}

std::optional<KernelVersion> KernelInterface::queryVersion() const {
    if (!isConnected()) {
        return std::nullopt;
    }

    AC_VERSION_INFO info{};
    DWORD bytes = 0;
    const BOOL ok = DeviceIoControl(
        handle_,
        IOCTL_AC_QUERY_VERSION,
        nullptr,
        0,
        &info,
        sizeof(info),
        &bytes,
        nullptr);

    if (!ok || bytes < sizeof(info)) {
        Logger::instance().warn(L"Kernel driver version query failed");
        return std::nullopt;
    }

    KernelVersion version{info.majorVersion, info.minorVersion};
    return version;
}

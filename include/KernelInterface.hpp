#pragma once

#include "KernelProtocol.h"

#include <optional>
#include <Windows.h>

struct KernelVersion {
    unsigned long major;
    unsigned long minor;
};

class KernelInterface {
public:
    KernelInterface();
    ~KernelInterface();

    bool connect();
    void disconnect();
    [[nodiscard]] bool isConnected() const;
    [[nodiscard]] std::optional<KernelVersion> queryVersion() const;

private:
    HANDLE handle_;
};

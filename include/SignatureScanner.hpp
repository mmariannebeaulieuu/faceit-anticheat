#pragma once

#include "ProcessEnumerator.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct SignatureHit {
    std::wstring signatureName;
    std::wstring moduleName;
    uintptr_t address;
};

class SignatureScanner {
public:
    SignatureScanner();

    [[nodiscard]] std::vector<SignatureHit> scan(const ProcessInfo& process) const;

private:
    struct Signature {
        std::wstring name;
        std::vector<uint8_t> pattern;
        std::vector<bool> mask;
    };

    [[nodiscard]] bool matchAt(const std::vector<uint8_t>& buffer, size_t index, const Signature& signature) const;

    std::vector<Signature> signatures_;
};

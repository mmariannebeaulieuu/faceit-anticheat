#pragma once

#include <string>
#include <string_view>
#include <vector>

struct CertificateVerification {
    bool trusted;
    bool publisherAllowed;
    long statusCode;
    std::wstring subjectName;
};

class CertificateVerifier {
public:
    CertificateVerifier();

    [[nodiscard]] CertificateVerification verifyImage(const std::wstring& path) const;
    [[nodiscard]] bool isPublisherAllowed(std::wstring_view subject) const;

private:
    std::vector<std::wstring> allowedSubjects_;
};

#include "CertificateVerifier.hpp"

#include <Windows.h>
#include <Wincrypt.h>
#include <Wintrust.h>
#include <Softpub.h>

#include <algorithm>
#include <array>
#include <cwctype>
#include <sstream>

namespace {
std::wstring toLower(std::wstring_view value) {
    std::wstring lowered(value.begin(), value.end());
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(std::towlower(ch));
    });
    return lowered;
}
}

CertificateVerifier::CertificateVerifier() {
    const std::vector<std::wstring> defaults = {
        L"Valve Corporation",
        L"Microsoft Corporation",
        L"FACEIT LTD",
    };
    allowedSubjects_.reserve(defaults.size());
    for (const auto& subject : defaults) {
        allowedSubjects_.push_back(toLower(subject));
    }
}

CertificateVerification CertificateVerifier::verifyImage(const std::wstring& path) const {
    CertificateVerification result{};
    if (path.empty()) {
        return result;
    }

    WINTRUST_FILE_INFO fileInfo{};
    fileInfo.cbStruct = sizeof(fileInfo);
    fileInfo.pcwszFilePath = path.c_str();

    WINTRUST_DATA trustData{};
    trustData.cbStruct = sizeof(trustData);
    trustData.dwUnionChoice = WTD_CHOICE_FILE;
    trustData.pFile = &fileInfo;
    trustData.dwUIChoice = WTD_UI_NONE;
    trustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    trustData.dwStateAction = WTD_STATEACTION_IGNORE;
    trustData.dwProvFlags = WTD_CACHE_ONLY_URL_RETRIEVAL | WTD_REVOCATION_CHECK_NONE;

    GUID policy = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    LONG status = WinVerifyTrust(nullptr, &policy, &trustData);
    result.statusCode = status;
    result.trusted = (status == ERROR_SUCCESS);

    DWORD encoding = 0;
    DWORD contentType = 0;
    DWORD formatType = 0;
    HCERTSTORE certStore = nullptr;
    HCRYPTMSG cryptMsg = nullptr;

    if (CryptQueryObject(
            CERT_QUERY_OBJECT_FILE,
            path.c_str(),
            CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
            CERT_QUERY_FORMAT_FLAG_BINARY,
            0,
            &encoding,
            &contentType,
            &formatType,
            &certStore,
            &cryptMsg,
            nullptr)) {
        PCCERT_CONTEXT context = CertEnumCertificatesInStore(certStore, nullptr);
        if (context != nullptr) {
            std::array<wchar_t, 512> subjectBuffer{};
            DWORD subjectLength = CertGetNameStringW(
                context,
                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                0,
                nullptr,
                subjectBuffer.data(),
                static_cast<DWORD>(subjectBuffer.size()));
            if (subjectLength > 1) {
                result.subjectName.assign(subjectBuffer.data(), subjectLength - 1);
                result.publisherAllowed = isPublisherAllowed(result.subjectName);
            }
            CertFreeCertificateContext(context);
        }
    }

    if (cryptMsg != nullptr) {
        CryptMsgClose(cryptMsg);
    }
    if (certStore != nullptr) {
        CertCloseStore(certStore, 0);
    }

    return result;
}

bool CertificateVerifier::isPublisherAllowed(std::wstring_view subject) const {
    if (subject.empty()) {
        return false;
    }
    const std::wstring normalized = toLower(subject);
    return std::find(allowedSubjects_.begin(), allowedSubjects_.end(), normalized) != allowedSubjects_.end();
}

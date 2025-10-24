#include "CertificateVerifier.hpp"
#include "KernelInterface.hpp"
#include "Logger.hpp"
#include "ProcessEnumerator.hpp"
#include "SignatureScanner.hpp"

#include <iomanip>
#include <sstream>

int wmain() {
    Logger::instance().info(L"CS:GO Anti-Cheat Prototype - Phase 3 initialization");

    KernelInterface kernel;
    if (kernel.connect()) {
        if (const auto version = kernel.queryVersion()) {
            std::wstringstream versionMsg;
            versionMsg << L"Kernel monitor ready version "
                       << version->major << L'.' << version->minor;
            Logger::instance().info(versionMsg.str());
        }
    }

    CertificateVerifier certificateVerifier;
    ProcessEnumerator enumerator;
    SignatureScanner scanner;
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
            continue;
        }

        if (!process.accessible) {
            builder << L" [ACCESS LIMITED]";
        } else {
            builder << L" [REVIEW]";
        }
        Logger::instance().warn(builder.str());

        if (process.accessible && !process.imagePath.empty()) {
            const auto verification = certificateVerifier.verifyImage(process.imagePath);
            std::wstringstream certMsg;
            certMsg << L"Certificate PID=" << process.pid << L" status=0x"
                    << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
                    << static_cast<unsigned long>(verification.statusCode);
            if (!verification.subjectName.empty()) {
                certMsg << L" subject=" << verification.subjectName;
            }
            if (verification.trusted && verification.publisherAllowed) {
                Logger::instance().info(certMsg.str());
            } else if (verification.trusted) {
                certMsg << L" [UNAPPROVED PUBLISHER]";
                Logger::instance().warn(certMsg.str());
            } else {
                Logger::instance().error(certMsg.str());
            }
        }

        const auto hits = scanner.scan(process);
        if (hits.empty()) {
            std::wstringstream noHitMsg;
            noHitMsg << L"No signature hits for PID=" << process.pid << L" (" << process.name << L")";
            Logger::instance().info(noHitMsg.str());
            continue;
        }

        for (const auto& hit : hits) {
            std::wstringstream hitMsg;
            hitMsg << L"Signature ['" << hit.signatureName << L"'] detected in module "
                   << hit.moduleName << L" at 0x"
                   << std::hex << std::uppercase
                   << std::setw(sizeof(uintptr_t) * 2)
                   << std::setfill(L'0')
                   << hit.address;
            Logger::instance().error(hitMsg.str());
        }
    }

    std::wstringstream summary;
    summary << L"Process enumeration complete. Total entries: " << processes.size();
    Logger::instance().info(summary.str());

    return 0;
}

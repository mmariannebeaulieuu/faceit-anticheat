# CI/CD Workflows for Anti-Cheat System

This directory contains comprehensive GitHub Actions workflows for a production-grade, kernel-level anti-cheat system with a focus on memory safety and security.

## 🔄 Workflow Overview

### Core Build & Test Workflows

#### 1. **build-and-test.yml**
**Triggers:** Push to main/develop, Pull Requests, Manual

Builds the anti-cheat client and launcher with multiple configurations:
- ✅ Release build of C++ client
- ✅ AddressSanitizer builds for memory error detection
- ✅ Python launcher compilation
- ✅ Static analysis with MSVC /analyze
- ✅ Artifact uploads with 30-day retention

**Key Features:**
- Windows-based builds with Visual Studio 2022
- Memory sanitizer instrumentation
- Build verification and artifact generation

---

#### 2. **security-scan.yml**
**Triggers:** Push, Pull Requests, Weekly schedule, Manual

Comprehensive security scanning:
- 🔒 CodeQL analysis for C++ and Python
- 🔒 Dependency vulnerability scanning (Safety, pip-audit)
- 🔒 Secret scanning with TruffleHog
- 🔒 Memory safety deep analysis
- 🔒 Kernel driver security patterns
- 🔒 Code signing readiness checks

**Security Checks:**
- Buffer overflow protection
- Input validation in kernel driver
- Unsafe function detection
- License compliance
- WHQL certification reminders

---

#### 3. **code-quality.yml**
**Triggers:** Push to main/develop, Pull Requests, Manual

Code quality and standards enforcement:
- 📊 C++ formatting with clang-format
- 📊 Python quality (Black, Flake8, Pylint, Bandit)
- 📊 Code complexity analysis with Lizard
- 📊 Documentation completeness checks
- 📊 MSVC maximum warning level builds
- 📊 Kernel driver best practices validation

**Quality Metrics:**
- Cyclomatic complexity (CCN threshold: 15)
- Coding standards compliance
- TODO/FIXME tracking
- SAL annotation checks for kernel code

---

### Memory Safety Workflows

#### 4. **memory-safety.yml**
**Triggers:** Push, Pull Requests, Daily schedule, Manual

Deep memory safety analysis:
- 🛡️ AddressSanitizer builds
- 🛡️ Buffer overflow detection
- 🛡️ Memory leak pattern analysis
- 🛡️ Null pointer dereference checks
- 🛡️ Integer overflow detection
- 🛡️ Use-after-free pattern detection
- 🛡️ Kernel memory safety validation

**Analysis Includes:**
- Unsafe function scanning (strcpy, sprintf, memcpy)
- malloc/free and new/delete pair validation
- Handle leak detection
- Kernel pool allocation validation
- IRQL-unsafe operation detection

---

### Pull Request Workflows

#### 5. **pr-checks.yml**
**Triggers:** Pull Requests (opened, synchronized, reopened)

Automated PR validation:
- ✔️ PR title convention enforcement (feat:, fix:, security:, etc.)
- ✔️ Breaking change detection
- ✔️ Binary size monitoring
- ✔️ Dependency change detection
- ✔️ Security-critical file change alerts
- ✔️ Commit message validation
- ✔️ Anti-cheat specific checks

**Anti-Cheat Specific Validation:**
- Signature exposure prevention
- Timing attack vulnerability checks
- Debug code detection
- Constant-time comparison enforcement

---

### Release & Deployment

#### 6. **release.yml**
**Triggers:** Version tags (v*.*.*), Manual

Production release pipeline:
- 📦 Release binary builds
- 📦 SHA256 checksum generation
- 📦 Release package creation
- 📦 GitHub release automation
- 📦 Code signing preparation guide
- 📦 Deployment checklist generation

**Release Artifacts:**
- Signed binaries (preparation)
- Checksums (SHA256SUMS.txt)
- Complete release package (.zip)
- Deployment checklist
- 90-day artifact retention

**Code Signing Requirements:**
- EV Certificate for user-mode executables
- Microsoft WHQL for kernel driver
- Timestamp signing for longevity

---

### Kernel-Specific Analysis

#### 7. **kernel-driver-analysis.yml**
**Triggers:** Changes to kernel/ or shared/, Pull Requests, Manual

Specialized kernel driver security:
- 🔐 Static analysis (Cppcheck, clang-tidy)
- 🔐 Pool allocation security validation
- 🔐 Buffer validation checks
- 🔐 Exception handling verification
- 🔐 IRQL compliance analysis
- 🔐 Race condition detection
- 🔐 IOCTL interface security
- 🔐 Attack surface analysis

**Kernel Safety Checks:**
- Pool tag enforcement
- ProbeForRead/ProbeForWrite validation
- Structured exception handling (__try/__except)
- Synchronization primitive usage
- SAL annotation presence
- Integer overflow in size calculations

---

### Continuous Monitoring

#### 8. **continuous-monitoring.yml**
**Triggers:** Every 6 hours, Manual

Ongoing codebase health monitoring:
- 📈 Dependency security audits
- 📈 Code churn analysis
- 📈 TODO/FIXME tracking
- 📈 Dead code detection
- 📈 Binary size tracking
- 📈 Build time monitoring
- 📈 License compliance
- 📈 Documentation drift detection

**Automated Actions:**
- Creates issues for vulnerabilities
- Tracks metrics over time
- Monitors technical debt

---

### Performance Testing

#### 9. **performance-testing.yml**
**Triggers:** Push, Pull Requests, Weekly schedule, Manual

Performance analysis and optimization:
- ⚡ Build performance tracking
- ⚡ Incremental build timing
- ⚡ Binary size optimization analysis
- ⚡ Startup time measurement
- ⚡ Memory footprint analysis
- ⚡ Code complexity impact
- ⚡ Link-time optimization (LTO) comparison
- ⚡ Header inclusion analysis

**Performance Metrics:**
- Build time benchmarks
- Binary size comparisons
- Memory usage tracking
- Compilation time optimization

---

### Nightly Builds

#### 10. **nightly-build.yml**
**Triggers:** Daily at 1 AM UTC, Manual

Comprehensive nightly validation:
- 🌙 All configuration builds (Debug/Release)
- 🌙 Cross-platform analysis
- 🌙 Extensive static analysis
- 🌙 Deep security scanning
- 🌙 Build statistics collection
- 🌙 Documentation generation
- 🌙 Anti-cheat validation tests

**Nightly Features:**
- Full Cppcheck analysis with XML output
- Doxygen documentation generation
- Comprehensive security deep dive
- 14-day artifact retention

---

### Dependency Management

#### 11. **dependency-update.yml**
**Triggers:** Weekly (Sundays), Manual

Automated dependency monitoring:
- 🔄 Python dependency updates
- 🔄 CMake version checks
- 🔄 GitHub Actions version tracking
- 🔄 Security advisory monitoring
- 🔄 Toolchain update checks
- 🔄 Update summary generation

---

## 📋 Workflow Triggers Summary

| Workflow | Push | PR | Schedule | Manual |
|----------|------|----|---------:|--------|
| Build & Test | ✅ | ✅ | ❌ | ✅ |
| Security Scan | ✅ | ✅ | Weekly | ✅ |
| Code Quality | ✅ | ✅ | ❌ | ✅ |
| Memory Safety | ✅ | ✅ | Daily | ✅ |
| PR Checks | ❌ | ✅ | ❌ | ❌ |
| Release | Tag | ❌ | ❌ | ✅ |
| Kernel Analysis | kernel/* | kernel/* | ❌ | ✅ |
| Monitoring | ❌ | ❌ | 6hrs | ✅ |
| Performance | ✅ | ✅ | Weekly | ✅ |
| Nightly | ❌ | ❌ | Daily | ✅ |
| Dependencies | ❌ | ❌ | Weekly | ✅ |

---

## 🔐 Security Features

### Memory Safety Focus
- **AddressSanitizer Integration**: Detects buffer overflows, use-after-free, memory leaks
- **Static Analysis**: MSVC /analyze, Cppcheck, clang-tidy
- **Pattern Detection**: Scans for unsafe functions (strcpy, sprintf, etc.)
- **Kernel Validation**: Specialized checks for kernel memory operations

### Vulnerability Detection
- **CodeQL**: Advanced semantic code analysis
- **Dependency Scanning**: Safety and pip-audit for Python
- **Secret Scanning**: TruffleHog for credential detection
- **License Compliance**: Automated license header checking

### Kernel Driver Security
- **IRQL Compliance**: Validates IRQL-safe operations
- **Input Validation**: Checks for ProbeForRead/ProbeForWrite
- **Pool Allocation**: Ensures tagged allocations
- **Exception Handling**: Verifies __try/__except usage
- **Attack Surface**: Analyzes exposed IOCTL interface

---

## 🚀 Getting Started

### Prerequisites
- Repository must be hosted on GitHub
- Windows runners for building (windows-latest)
- No additional secrets required for basic functionality

### Enabling Workflows
1. Workflows are automatically enabled when pushed to repository
2. First run may require approval in GitHub Settings → Actions
3. Review and adjust schedules in workflow files as needed

### Required Permissions
```yaml
permissions:
  contents: write          # For release creation
  security-events: write   # For CodeQL
  issues: write           # For automated issue creation
  pull-requests: write    # For PR comments
```

---

## 📊 Artifact Management

### Retention Policies
- **Build Artifacts**: 30 days
- **Release Artifacts**: 90 days
- **Analysis Reports**: 14-30 days
- **Nightly Builds**: 14 days

### Artifact Types
- Compiled binaries (Release/Debug)
- Static analysis reports (XML, TXT)
- Performance metrics (CSV)
- Security scan results (JSON)
- Documentation (HTML)

---

## 🛠️ Customization

### Adjusting Schedules
```yaml
schedule:
  - cron: '0 1 * * *'  # Daily at 1 AM UTC
```

[Cron Expression Helper](https://crontab.guru/)

### Adding New Checks
1. Create new workflow file in `.github/workflows/`
2. Follow existing naming conventions
3. Add to this README documentation
4. Test with workflow_dispatch trigger

### Modifying Security Thresholds
Edit workflow files to adjust:
- Complexity thresholds (default CCN: 15)
- Binary size limits (default: 10MB warning)
- Build time alerts (default: 120s)

---

## 📖 Best Practices

### For Developers
1. **Run locally before pushing**: Use provided build scripts
2. **Address CI failures promptly**: Security and memory safety are critical
3. **Review PR checks**: Don't ignore warnings in security-critical code
4. **Update documentation**: Keep docs in sync with code changes

### For Security Reviews
1. **Check kernel-driver-analysis results** for kernel changes
2. **Review memory-safety workflow** for memory-related PRs
3. **Validate security-scan results** before releases
4. **Verify code signing preparation** in release workflow

### For Releases
1. **Always use version tags**: Follow semver (v1.0.0)
2. **Review deployment checklist**: Generated in release workflow
3. **Sign binaries**: Follow code signing instructions
4. **Update changelog**: In CHANGES.md before tagging

---

## 🐛 Troubleshooting

### Common Issues

**Build Failures on Windows**
- Ensure Visual Studio 2022 components installed
- Check CMake version (3.20+)
- Verify Windows SDK availability

**CodeQL Analysis Timeouts**
- Large codebases may need timeout adjustments
- Consider splitting analysis by language

**Artifact Upload Failures**
- Check artifact size limits (GitHub: 2GB per artifact)
- Verify path specifications in workflow

**Scheduled Workflows Not Running**
- Repository must have recent activity
- Schedules can be delayed during high load

---

## 📚 Additional Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [CodeQL for C/C++](https://codeql.github.com/docs/codeql-language-guides/codeql-for-cpp/)
- [Windows Driver Security](https://docs.microsoft.com/en-us/windows-hardware/drivers/driversecurity/)
- [AddressSanitizer (ASan)](https://docs.microsoft.com/en-us/cpp/sanitizers/asan)
- [Code Signing for Windows](https://docs.microsoft.com/en-us/windows/win32/seccrypto/cryptography-tools)

---

## 🤝 Contributing

When adding or modifying workflows:
1. Test thoroughly with workflow_dispatch
2. Document changes in this README
3. Consider impact on build times
4. Maintain security-first approach
5. Update artifact retention policies as needed

---

## 📝 Maintenance

### Regular Tasks
- **Weekly**: Review security advisories
- **Monthly**: Update GitHub Actions versions
- **Quarterly**: Review and adjust schedules
- **Per Release**: Verify all checks pass

### Workflow Health
- Monitor workflow run times
- Review artifact storage usage
- Check for deprecated actions
- Update tool versions (Cppcheck, etc.)

---

## 🎯 Goals & Metrics

### Coverage Goals
- ✅ 100% of PRs validated
- ✅ Daily security scanning
- ✅ Nightly comprehensive builds
- ✅ Weekly dependency audits

### Quality Metrics
- Build success rate: Target 95%+
- Security scan findings: Target 0 high-severity
- Code complexity: Target CCN < 15
- Test coverage: Implement testing framework

---

**Last Updated**: 2024
**Maintained by**: DevOps Team
**Contact**: See repository maintainers

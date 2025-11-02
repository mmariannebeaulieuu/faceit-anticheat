# GitHub Actions Quick Start Guide

## 🚀 Getting Started

Your repository now has **11 production-ready CI/CD workflows** specifically designed for kernel-level anti-cheat development with a focus on memory safety.

## ✅ First Steps After Merge

1. **Verify Workflows are Active**
   - Go to: `Repository → Actions tab`
   - You should see all 11 workflows listed
   - First runs may require approval in repository settings

2. **Review Security Settings**
   - Go to: `Repository → Settings → Security`
   - Enable: Code scanning alerts
   - Enable: Dependabot alerts
   - Enable: Secret scanning

3. **Configure Branch Protection**
   - Go to: `Repository → Settings → Branches`
   - Add rule for `main` branch:
     - ✅ Require status checks to pass
     - ✅ Require branches to be up to date
     - Select: `build-client`, `security-final-check`

## 📋 Workflow Status Quick View

After pushing, check the Actions tab to see:

```
✓ Build and Test          - Main build pipeline
✓ Security Scanning       - CodeQL + vulnerability detection  
✓ Code Quality           - Linting and formatting
✓ Memory Safety          - Deep memory analysis
✓ PR Checks              - Automated PR validation
```

## 🔔 What Happens Automatically

### On Every Push to `main` or `develop`:
- ✅ Full build (Release + Debug)
- ✅ Security scanning
- ✅ Code quality checks
- ✅ Memory safety analysis
- ✅ Performance testing

### On Every Pull Request:
- ✅ PR title validation
- ✅ Code formatting checks
- ✅ Security critical file detection
- ✅ Binary size monitoring
- ✅ Commit message validation

### On Every Release Tag (`v*.*.*`):
- ✅ Production builds
- ✅ Checksum generation
- ✅ GitHub release creation
- ✅ Deployment checklist

### Scheduled Automatically:
- 🕐 **Every 6 hours**: Dependency security audit
- 🌙 **Daily (1 AM)**: Comprehensive nightly build
- 📅 **Weekly (Monday)**: Full security scan
- 📅 **Weekly (Sunday)**: Dependency update check

## 🎯 Quick Actions

### Run a Workflow Manually
1. Go to: `Actions tab`
2. Select workflow from left sidebar
3. Click: `Run workflow` button
4. Choose branch and click: `Run workflow`

### Check Latest Build Status
```bash
# View all workflows
https://github.com/YOUR_ORG/YOUR_REPO/actions

# View specific workflow
https://github.com/YOUR_ORG/YOUR_REPO/actions/workflows/build-and-test.yml
```

### Download Build Artifacts
1. Go to: `Actions tab`
2. Click on workflow run
3. Scroll to: `Artifacts` section
4. Download: `anticheat-client-windows-x64`

## ⚠️ Important Checks

### Before Creating a PR:
```bash
# Ensure your branch is clean
git status

# Your PR title should start with:
# feat: for new features
# fix: for bug fixes
# security: for security fixes
# refactor: for code refactoring
```

### Before Releasing:
```bash
# Create a version tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# This triggers the release pipeline automatically
```

## 🔐 Security Workflows

### CodeQL Analysis
- **Runs**: On every push and PR
- **Languages**: C++ and Python
- **Results**: Security tab → Code scanning alerts

### Memory Safety Checks
- **Runs**: Daily + on changes
- **Detects**: Buffer overflows, use-after-free, memory leaks
- **Tool**: AddressSanitizer + pattern analysis

### Kernel Driver Security
- **Runs**: When kernel/ files change
- **Checks**: IRQL compliance, pool allocation, input validation
- **Focus**: Production kernel driver safety

## 📊 Viewing Reports

### Build Artifacts
- **Location**: Actions → Workflow Run → Artifacts
- **Includes**: Binaries, analysis logs, reports
- **Retention**: 14-90 days (varies by type)

### Security Alerts
- **Location**: Security tab → Code scanning
- **Types**: CodeQL findings, vulnerabilities
- **Action**: Review and fix before merging

### Performance Metrics
- **Location**: Actions → performance-testing workflow
- **Includes**: Build times, binary sizes, memory usage
- **Frequency**: Weekly + on changes

## 🐛 Troubleshooting

### "Workflow requires approval"
- **Solution**: Repository Settings → Actions → General
- Enable: "Allow all actions and reusable workflows"

### "Build failed on Windows"
- **Check**: Build logs in workflow run
- **Common**: Missing Visual Studio components
- **Solution**: Workflows use windows-latest (includes VS 2022)

### "CodeQL analysis timed out"
- **Reason**: Large codebase analysis
- **Solution**: Automatic retry or adjust timeout in workflow

### "Artifact upload failed"
- **Check**: Artifact size (limit: 2GB)
- **Solution**: Reduce artifact size or split uploads

## 📚 Documentation

### Full Documentation
- **Workflows**: `.github/workflows/README.md`
- **Implementation**: `CI_CD_IMPLEMENTATION.md`
- **This Guide**: `.github/QUICK_START.md`

### Key Files
```
.github/
├── workflows/
│   ├── README.md                    ← Complete workflow documentation
│   ├── build-and-test.yml          ← Main build pipeline
│   ├── security-scan.yml           ← Security scanning
│   ├── memory-safety.yml           ← Memory analysis
│   ├── kernel-driver-analysis.yml  ← Kernel specific
│   └── ... (7 more workflows)
└── QUICK_START.md                   ← This file

CI_CD_IMPLEMENTATION.md              ← Implementation details
```

## 🎓 Best Practices

### 1. Before Pushing Code
```bash
# Run local build
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# Check for common issues
git diff --check
```

### 2. Writing Commit Messages
```
Good:
✅ feat: add memory pool validation in kernel driver
✅ fix: prevent buffer overflow in signature scanner
✅ security: add input validation for IOCTL handler

Bad:
❌ update code
❌ fix bug
❌ WIP
```

### 3. Responding to CI Failures
1. **Read the error message** in workflow logs
2. **Check which workflow failed** (build, security, quality)
3. **Fix the issue** and push again
4. **Don't disable checks** - they protect production

## 🚨 Critical Alerts

### These Require Immediate Attention:
- 🔴 **Security scan failures**: Review immediately
- 🔴 **Kernel driver analysis errors**: Must fix before merge
- 🔴 **Memory safety violations**: Critical for anti-cheat
- 🟡 **Code quality warnings**: Should fix soon
- 🟡 **Performance regressions**: Monitor and optimize

## 💡 Tips

### Speed Up Feedback
- Use **workflow_dispatch** to trigger workflows manually
- Check **Actions tab** immediately after pushing
- Download **artifacts** to test locally

### Reduce Build Times
- Use **incremental builds** when possible
- Keep **dependencies minimal**
- Review **performance-testing** workflow results

### Improve Code Quality
- Address **CodeQL suggestions** proactively
- Keep **complexity low** (CCN < 15)
- Add **unit tests** (framework to be added)

## 📞 Support

### Issues with Workflows?
1. Check workflow logs in Actions tab
2. Review `.github/workflows/README.md`
3. Consult `CI_CD_IMPLEMENTATION.md`
4. Create issue with logs attached

### Need to Customize?
- All workflows support manual triggers
- Thresholds can be adjusted in YAML files
- Schedules can be modified per requirement
- Additional checks can be added

## 🎉 Success Indicators

Your CI/CD is working correctly when you see:

✅ **Green checkmarks** on all PR checks  
✅ **No security alerts** in Security tab  
✅ **Build artifacts** generated automatically  
✅ **Nightly builds** completing successfully  
✅ **Performance metrics** tracked over time  

---

**Quick Links**:
- [Actions Tab](../../actions)
- [Security Alerts](../../security)
- [Workflow Documentation](.github/workflows/README.md)
- [Implementation Details](../CI_CD_IMPLEMENTATION.md)

---

**Need Help?** Check the full documentation in `.github/workflows/README.md`

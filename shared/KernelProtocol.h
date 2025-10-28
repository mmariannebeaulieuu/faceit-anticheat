#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define AC_DEVICE_NAME L"\\Device\\AcKernelMonitor"
#define AC_DOS_DEVICE_NAME L"\\DosDevices\\AcKernelMonitor"
#define AC_USER_DEVICE_PATH L"\\\\.\\AcKernelMonitor"

#define AC_VERSION_MAJOR 1
#define AC_VERSION_MINOR 0

typedef struct _AC_VERSION_INFO {
    unsigned long majorVersion;
    unsigned long minorVersion;
} AC_VERSION_INFO, *PAC_VERSION_INFO;

#define IOCTL_AC_QUERY_VERSION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

#ifdef __cplusplus
}
#endif

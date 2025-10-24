#include <ntddk.h>

#include "../shared/KernelProtocol.h"

static PDEVICE_OBJECT gDeviceObject = NULL;

static NTSTATUS AcDriverCreateClose(PDEVICE_OBJECT deviceObject, PIRP irp) {
    UNREFERENCED_PARAMETER(deviceObject);
    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

static NTSTATUS AcDriverDeviceControl(PDEVICE_OBJECT deviceObject, PIRP irp) {
    UNREFERENCED_PARAMETER(deviceObject);
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);
    ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;

    if (code == IOCTL_AC_QUERY_VERSION) {
        if (stack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(AC_VERSION_INFO)) {
            status = STATUS_BUFFER_TOO_SMALL;
            irp->IoStatus.Information = 0;
        } else {
            PAC_VERSION_INFO version = (PAC_VERSION_INFO)irp->AssociatedIrp.SystemBuffer;
            version->majorVersion = AC_VERSION_MAJOR;
            version->minorVersion = AC_VERSION_MINOR;
            irp->IoStatus.Information = sizeof(AC_VERSION_INFO);
            status = STATUS_SUCCESS;
        }
    } else {
        irp->IoStatus.Information = 0;
    }

    irp->IoStatus.Status = status;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

static VOID AcDriverUnload(PDRIVER_OBJECT driverObject) {
    UNICODE_STRING symLink;
    RtlInitUnicodeString(&symLink, AC_DOS_DEVICE_NAME);
    IoDeleteSymbolicLink(&symLink);

    if (gDeviceObject != NULL) {
        IoDeleteDevice(gDeviceObject);
        gDeviceObject = NULL;
    }

    UNREFERENCED_PARAMETER(driverObject);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
    UNREFERENCED_PARAMETER(registryPath);

    UNICODE_STRING deviceName;
    RtlInitUnicodeString(&deviceName, AC_DEVICE_NAME);

    NTSTATUS status = IoCreateDevice(
        driverObject,
        0,
        &deviceName,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &gDeviceObject);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    UNICODE_STRING symLink;
    RtlInitUnicodeString(&symLink, AC_DOS_DEVICE_NAME);

    status = IoCreateSymbolicLink(&symLink, &deviceName);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(gDeviceObject);
        gDeviceObject = NULL;
        return status;
    }

    driverObject->MajorFunction[IRP_MJ_CREATE] = AcDriverCreateClose;
    driverObject->MajorFunction[IRP_MJ_CLOSE] = AcDriverCreateClose;
    driverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = AcDriverDeviceControl;
    driverObject->DriverUnload = AcDriverUnload;

    return STATUS_SUCCESS;
}

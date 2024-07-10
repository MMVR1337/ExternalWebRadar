#include <ntifs.h>
#include <ntddk.h>

#define SERVER_PORT 28000

typedef struct _READ_MEMORY_REQUEST {
    ULONG ProcessId;
    PVOID Address;
    SIZE_T Size;
} READ_MEMORY_REQUEST, * PREAD_MEMORY_REQUEST;

void UnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("Driver Unloaded\n");
}

NTSTATUS UnsupportedDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_NOT_SUPPORTED;
}

NTSTATUS ReadMemoryHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);

    PREAD_MEMORY_REQUEST request = (PREAD_MEMORY_REQUEST)Irp->AssociatedIrp.SystemBuffer;

    PEPROCESS process;
    NTSTATUS status = PsLookupProcessByProcessId((HANDLE)request->ProcessId, &process);
    if (!NT_SUCCESS(status)) {
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    }

    PVOID buffer = ExAllocatePool(NonPagedPool, request->Size);
    if (buffer == NULL) {
        Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    KAPC_STATE state;
    KeStackAttachProcess(process, &state);
    __try {
        RtlCopyMemory(buffer, request->Address, request->Size);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        status = GetExceptionCode();
    }
    KeUnstackDetachProcess(&state);

    if (NT_SUCCESS(status)) {
        RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, buffer, request->Size);
        Irp->IoStatus.Information = request->Size;
    }
    else {
        Irp->IoStatus.Information = 0;
    }

    ExFreePool(buffer);
    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = UnloadDriver;

    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
        DriverObject->MajorFunction[i] = UnsupportedDispatch;
    }
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ReadMemoryHandler;

    DbgPrint("Driver Loaded\n");

    return STATUS_SUCCESS;
}

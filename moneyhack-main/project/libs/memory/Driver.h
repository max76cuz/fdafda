#include <iostream>
#include <Windows.h>
#include <memory>
#include <algorithm>
#include <TlHelp32.h>
#include <string>

#include "nt.h"

typedef NTSTATUS(NTAPI* NtQuerySystemInformationPTR)(
    _In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Out_writes_bytes_opt_(SystemInformationLength) PVOID SystemInformation,
    _In_ ULONG SystemInformationLength,
    _Out_opt_ PULONG ReturnLength
    );


typedef NTSTATUS(NTAPI* NtDeviceIoControlFilePTR)(
        _In_ HANDLE FileHandle,
        _In_opt_ HANDLE Event,
        _In_opt_ PIO_APC_ROUTINE ApcRoutine,
        _In_opt_ PVOID ApcContext,
        _Out_ PIO_STATUS_BLOCK IoStatusBlock,
        _In_ ULONG IoControlCode,
        _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
        _In_ ULONG InputBufferLength,
        _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
        _In_ ULONG OutputBufferLength
    );

#define GENERIC_RW (GENERIC_READ | GENERIC_WRITE)
#define FILE_SHARE_RW (FILE_SHARE_READ | FILE_SHARE_WRITE)

const ULONG C_DMA CTL_CODE(FILE_DEVICE_UNKNOWN, 0x591, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
const ULONG C_CLOSE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x592, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)


class vDriver
{
private:
    HANDLE t_handle = 0;
    int t_pid = 0;
    const int t_key = 814957;


    struct Request {
        int t_key;
        INT32 t_PID;
        ULONGLONG VA;
        ULONGLONG BUFFER;
        ULONGLONG Size;
    };

    NtDeviceIoControlFilePTR NtFunction = 0;
    NtQuerySystemInformationPTR NtQuery = 0;

public:
    bool OpenHandle();
    bool CloseHandle();
    bool UnloadDriver();
    bool Attach(const std::wstring& ProcessName);
    int GetProcessPid(const std::wstring& ProcessName);
    uint64_t GetKernelModuleBase(const char* ModuleName);
    uint64_t GetModuleBaseAddress(const std::wstring& ModuleName);
    bool ReadPhysicalMemory(PVOID Address, PVOID Buffer, SIZE_T BufferSize);
    std::string read_string(uintptr_t addr, size_t max_length = 256);

    template <typename BUF> BUF Read(uint64_t Address);
};


inline vDriver* Driver = new vDriver;
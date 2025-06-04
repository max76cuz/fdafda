#include "Driver.h"

bool vDriver::OpenHandle()
{
    t_handle = CreateFileW(L"\\\\.\\green", GENERIC_RW, FILE_SHARE_RW, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN, NULL);
    return (t_handle != INVALID_HANDLE_VALUE);
}

bool vDriver::CloseHandle()
{
    if (t_handle != INVALID_HANDLE_VALUE) {
        return (::CloseHandle(t_handle) != 0);
    }
    return false;
}

bool vDriver::UnloadDriver()
{
    return DeviceIoControl(t_handle, C_CLOSE, nullptr, 0, nullptr, 0, 0, nullptr);
}

bool vDriver::Attach(const std::wstring& ProcessName)
{
    t_pid = GetProcessPid(ProcessName);
    if (t_pid == 0)
        return false;

    HMODULE NTDLL = GetModuleHandleA("ntdll.dll");
    if (NTDLL) {
        NtFunction = (NtDeviceIoControlFilePTR)GetProcAddress(NTDLL, "NtDeviceIoControlFile");
        NtQuery = (NtQuerySystemInformationPTR)GetProcAddress(NTDLL, "NtQuerySystemInformation");
    }

    return true;
}

int vDriver::GetProcessPid(const std::wstring& ProcessName)
{
    if (!NtQuery)
        return 0;

    ULONG bufferSize = 1024 * 1024;
    std::unique_ptr<BYTE[]> buffer(new BYTE[bufferSize]);
    ULONG returnLength = 0;

    NTSTATUS status = NtQuery(SystemProcessInformation, buffer.get(), bufferSize, &returnLength);
    if (status != STATUS_SUCCESS)
        return 0;

    SYSTEM_PROCESS_INFORMATION* processInfo = (SYSTEM_PROCESS_INFORMATION*)(buffer.get());
    while (processInfo) {
        std::wstring processNameStr(processInfo->ImageName.Buffer, processInfo->ImageName.Length / sizeof(wchar_t));

        if (processNameStr == ProcessName) {
            return reinterpret_cast<int>(t_handle);
        }

        if (processInfo->NextEntryOffset == 0)
            break;

        processInfo = (SYSTEM_PROCESS_INFORMATION*)(((BYTE*)processInfo) + processInfo->NextEntryOffset);
    }
    return 0;
}

uint64_t vDriver::GetKernelModuleBase(const char* ModuleName)
{
    if (!NtQuery)
        return 0;

    ULONG bufferSize = 1024 * 1024;
    std::unique_ptr<BYTE[]> buffer(new BYTE[bufferSize]);
    ULONG returnLength = 0;

    NTSTATUS status = NtQuery(SystemModuleInformation, buffer.get(), bufferSize, &returnLength);
    if (status != STATUS_SUCCESS)
        return 0;

    RTL_PROCESS_MODULES* ModuleInfo = reinterpret_cast<RTL_PROCESS_MODULES*>(buffer.get());
    ULONG Size = ModuleInfo->NumberOfModules;

    for (ULONG i = 0; i < Size; i++) {
        std::string fullPath(reinterpret_cast<char*>(ModuleInfo->Modules[i].FullPathName));
        if (fullPath.find(ModuleName) != std::string::npos) {
            return (uintptr_t)ModuleInfo->Modules[i].ImageBase;
        }
    }

    return 0;
}

uint64_t vDriver::GetModuleBaseAddress(const std::wstring& ModuleName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, t_pid);
    if (!hSnapshot)
        return 0;

    MODULEENTRY32 me32 = { 0 };
    me32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &me32)) {
        do {
            if (ModuleName == me32.szModule) {
                ::CloseHandle(hSnapshot);
                return (uint64_t)me32.modBaseAddr;
            }
        } while (Module32Next(hSnapshot, &me32));
    }

    ::CloseHandle(hSnapshot);
    return 0;
}

bool vDriver::ReadPhysicalMemory(PVOID Address, PVOID Buffer, SIZE_T BufferSize)
{
    Request In = {};
    In.t_key = t_key;
    In.t_PID = t_pid;
    In.VA = (ULONGLONG)Address;
    In.BUFFER = (ULONGLONG)Buffer;
    In.Size = BufferSize;

    IO_STATUS_BLOCK IoStatusBlock = {};
    NtFunction(t_handle, nullptr, nullptr, nullptr, &IoStatusBlock, C_DMA, &In, sizeof(In), 0, 0);

    return (IoStatusBlock.Status == STATUS_SUCCESS);
}

std::string vDriver::read_string(uintptr_t addr, size_t max_length)
{
    char buffer[256];
    size_t length_to_read = min(max_length, sizeof(buffer) / sizeof(char));
    ReadPhysicalMemory((PVOID)addr, buffer, length_to_read);
    buffer[length_to_read - 1] = '\0';
    std::string result(buffer);
    result.erase(result.find_last_not_of('\0') + 1);
    return result;
}

template <typename BUF> BUF vDriver::Read(uint64_t Address)
{
    BUF TMP = {};
    ReadPhysicalMemory((PVOID)Address, &TMP, sizeof(BUF));
    return TMP;
}
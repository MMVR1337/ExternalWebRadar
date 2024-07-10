#include <windows.h>
#include <iostream>
#include <string>
#include <iostream>


class MemoryReader {
public:
    MemoryReader(DWORD pid) : pid(pid), hProcess(NULL) {
        hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (hProcess == NULL) {
            std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        }
    }

    ~MemoryReader() {
        if (hProcess != NULL) {
            CloseHandle(hProcess);
        }
    }

    template <typename T>
    T read(uintptr_t address) {
        T value;
        SIZE_T bytesRead;
        if (ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), &bytesRead) && bytesRead == sizeof(T)) {
            return value;
        }
        else {
            std::cerr << "Failed to read memory. Error: " << GetLastError() << std::endl;
            return T();
        }
    }

    bool isValid() const {
        return hProcess != NULL;
    }

private:
    DWORD pid;
    HANDLE hProcess;
};
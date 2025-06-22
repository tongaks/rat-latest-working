#include "socket.h"

bool Client::CheckProcess(std::string proc) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };

    while (Process32Next(snapshot, &entry)) {
        if (proc == entry.szExeFile) {
            CloseHandle(snapshot);
            return true;
        }
    }

    CloseHandle(snapshot);
    return false;
}
//
// Created by tomato on 2021/01/17.
//
#include "helper.h"
#include <Psapi.h>

const wchar_t *getWC(const char *c) {
    const size_t cSize = strlen(c) + 1;
    auto *wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);

    return wc;
}

DWORD getProcessIDByName(const std::wstring &processName) {
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(getWC(processInfo.szExeFile))) {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo)) {
        if (processName == getWC(processInfo.szExeFile)) {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

DWORD_PTR getProcessBaseAddress(DWORD_PTR processID) {
    DWORD_PTR baseAddress = 0;
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    HMODULE *moduleArray;
    LPBYTE moduleArrayBytes;
    DWORD bytesRequired;

    if (processHandle) {
        if (EnumProcessModules(processHandle, nullptr, 0, &bytesRequired)) {
            if (bytesRequired) {
                moduleArrayBytes = (LPBYTE) LocalAlloc(LPTR, bytesRequired);

                if (moduleArrayBytes) {
                    unsigned int moduleCount;

                    moduleCount = bytesRequired / sizeof(HMODULE);
                    moduleArray = (HMODULE *) moduleArrayBytes;

                    if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired)) {
                        baseAddress = (DWORD_PTR) moduleArray[0];
                    }

                    LocalFree(moduleArrayBytes);
                }
            }
        }

        CloseHandle(processHandle);
    }

    return baseAddress;
}

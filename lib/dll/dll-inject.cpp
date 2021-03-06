//
// Created by tomato on 2021/01/17.
//
#include <iostream>
#include <stdexcept>
#include "dll-inject.h"
#include "../helper/helper.h"

using namespace std;

//判断某模块(dllPath)是否在相应的进程中
//dwPID         进程的PID
//dllPath     查询的dll的完整路径
bool checkDllInProcess(DWORD dwPID, WCHAR *dllPath) {
    HANDLE hSnapshot = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me = {sizeof(me),};

    if (INVALID_HANDLE_VALUE ==
        (hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)))//获得进程的快照
    {
        _tprintf("checkDllInProcess() : CreateToolhelp32Snapshot(%lu) failed!!! [%lu]\n",
                 dwPID, GetLastError());
        return false;
    }
    bool bMore = Module32First(hSnapshot, &me);//遍历进程内得的所有模块
    for (; bMore; bMore = Module32Next(hSnapshot, &me)) {
        cout << "szModule: " << me.szModule << endl;
        if (!wcscmp(getWC(me.szModule), dllPath) || !wcscmp(getWC(me.szExePath), dllPath))//模块名或含路径的名相符
        {
            CloseHandle(hSnapshot);
            return true;
        }
    }
    CloseHandle(hSnapshot);
    return false;
}

//让指定的进程卸载相应的模块
//dwPID         目标进程的PID
//dllPath     被注入的dll的完整路径,注意：路径不要用“/”来代替“\\”
bool ejectDll(DWORD dwPID, WCHAR *dllPath) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    MODULEENTRY32 me = {sizeof(me),};
    HANDLE hProcess = nullptr;

    if (INVALID_HANDLE_VALUE == hSnapshot) {
        return false;
    }

    try {
        bool bFound = false;
        for (bool bMore = Module32First(hSnapshot, &me); bMore; bMore = Module32Next(hSnapshot, &me))//查找模块句柄
        {
            if (!wcscmp(getWC(me.szModule), dllPath) ||
                !wcscmp(getWC(me.szExePath), dllPath)) {
                bFound = true;
                break;
            }
        }
        if (!bFound) {
            throw std::invalid_argument("EjectDll() : There is not module in process memory!!!");
        }
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);
        if (!hProcess) {
            throw std::invalid_argument("EjectDll() : OpenProcess failed!!!");
        }
        HMODULE hMod = GetModuleHandle("kernel32.dll");
        if (hMod == nullptr) {
            throw std::invalid_argument("EjectDll() : GetModuleHandle(\"kernel32.dll\") failed!!! [%lu]");
        }

        auto pThreadProc = (LPTHREAD_START_ROUTINE) GetProcAddress(hMod, "FreeLibrary");
        if (pThreadProc == nullptr) {
            throw std::invalid_argument("EjectDll() : GetProcAddress(\"FreeLibrary\") failed!!!");
        }
        if (!CreateRemoteThread(hProcess, nullptr, 0, pThreadProc, me.modBaseAddr, 0, nullptr)) {
            throw std::invalid_argument("EjectDll() : MyCreateRemoteThread() failed!!!");
        }

        CloseHandle(hProcess);
        CloseHandle(hSnapshot);
        return true;
    } catch (const std::exception& e) {
        if (hProcess)
            CloseHandle(hProcess);
        if (hSnapshot != INVALID_HANDLE_VALUE)
            CloseHandle(hSnapshot);
        return false;
    }
}

//向指定的进程注入相应的模块
//dwPID         目标进程的PID
//dllPath     被注入的dll的完整路径
bool injectDll(DWORD dwPID, WCHAR *dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID); //保存目标进程的句柄
    const unsigned int dllPathSize = (wcslen(dllPath) + 1) * sizeof(WCHAR); //开辟的内存的大小

    HMODULE hMod = GetModuleHandle("kernel32.dll");//获得本进程kernel32.dll的模块句柄
    FARPROC pThreadProc = GetProcAddress(hMod, "LoadLibraryW");//获得LoadLibraryW函数的起始地址

    LPVOID pRemoteBuf = VirtualAllocEx(hProcess, nullptr, dllPathSize,
                                       MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // 目标进程开辟的内存的起始地址，在目标进程空间开辟一块内存

    WriteProcessMemory(hProcess, pRemoteBuf, dllPath, dllPathSize, nullptr);

    HANDLE thread = CreateRemoteThread(hProcess, nullptr, 0,
                                       reinterpret_cast<LPTHREAD_START_ROUTINE>(pThreadProc), pRemoteBuf, 0, nullptr);
    WaitForSingleObject(thread, INFINITE);
    bool bRet = checkDllInProcess(dwPID, dllPath);//确认结果
    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return bRet;
}

//
// Created by tomato on 2021/01/17.
//

#include "Windows.h"
#include <cstdio>
#include "TlHelp32.h"
#include "tchar.h"
#include "iostream"
#include <process.h>

#ifndef SHARED_DLL_DLL_INJECT_H
#define SHARED_DLL_DLL_INJECT_H

bool ejectDll(DWORD dwPID, WCHAR *dllPath);
bool injectDll(DWORD dwPID, WCHAR *dllPath);
bool checkDllInProcess(DWORD dwPID, WCHAR *dllPath);

#endif //SHARED_DLL_DLL_INJECT_H

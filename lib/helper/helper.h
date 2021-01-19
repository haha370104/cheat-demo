//
// Created by tomato on 2021/01/17.
//
#include "Windows.h"
#include <cstdio>
#include "TlHelp32.h"
#include "tchar.h"
#include "iostream"
#include <process.h>

#ifndef SHARED_DLL_HELPER_H
#define SHARED_DLL_HELPER_H

const wchar_t *getWC(const char *c);
DWORD getProcessIDByName(const std::wstring &processName);
DWORD_PTR getProcessBaseAddress(DWORD_PTR processID);

#endif //SHARED_DLL_HELPER_H

#include "library.h"
#include <iostream>
#include <Windows.h>
#include "lib/helper/helper.h"

void hello2() {
    std::cout << "Hello, World!" << std::endl;
}

// 0x4004 + 基址地址

int getA() {
    auto processID = GetCurrentProcessId();
    int &p = *(int *) (getProcessBaseAddress(processID) + 0x4004);
    return p;
}

BOOL WINAPI DllMain(HMODULE hModule,
                    DWORD ul_reason_for_call,
                    LPVOID lpReserved
) {
    // Perform actions based on the reason for calling.
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            std::cout << "dll attach" << std::endl;
            std::cout << "the global variable is: " << getA() << std::endl;
            break;

        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            std::cout << "dll thread detach" << std::endl;
            break;

        case DLL_PROCESS_DETACH:
            std::cout << "dll process detach" << std::endl;
            // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

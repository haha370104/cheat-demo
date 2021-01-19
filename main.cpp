#include <Windows.h>
#include <iostream>

using namespace std;

//int main() {
//    HINSTANCE hGetProcIDDLL = LoadLibrary(TEXT(".\\shared_dll.dll"));
//
//    if (hGetProcIDDLL == NULL) {
//        cout << "DLL could not be loaded. " << GetLastError() << endl;
//        return -1;
//    }
//
//    auto f = GetProcAddress(hGetProcIDDLL, "hello2");
//
//    if (f == NULL) {
//        cout << "Factory function could not be resolved. " << GetLastError() << endl;
//        return -1;
//    }
//
//    cout << "Factory function returns: " << f() << endl;
//}
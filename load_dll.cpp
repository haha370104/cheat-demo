#include "lib/helper/helper.h"
#include "lib/dll/dll-inject.h"

using namespace std;

int main() {
    int pid = getProcessIDByName(L"test1.exe");
    WCHAR *dll_path = L"C:\\Users\\tomato\\CLionProjects\\shared-dll\\cmake-build-debug-visual-studio\\shared_dll.dll";
    injectDll(pid, dll_path);
    ejectDll(pid, dll_path);
    cout << "dll has unloaded" << endl;
    checkDllInProcess(pid, dll_path);
}

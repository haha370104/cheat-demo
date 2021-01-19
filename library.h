#ifndef SHARED_DLL_LIBRARY_H
#define SHARED_DLL_LIBRARY_H

#ifndef EXPORT
#define EXPORT __declspec(dllexport)
#endif

extern "C" {
EXPORT int dummy() {
    return 5;
}

EXPORT void hello2();
}

#endif //SHARED_DLL_LIBRARY_H

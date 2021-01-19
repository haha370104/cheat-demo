cmake_minimum_required(VERSION 3.17)
project(shared_dll)

set(CMAKE_CXX_STANDARD 14)

set(LIB_SOURCE_FILES lib/helper/helper.h lib/helper/helper.cpp)
set(DLL_INJECT_LIB_SOURCE_FILES lib/dll/dll-inject.h lib/dll/dll-inject.cpp)

set(DLL_SOURCE_FILES ${LIB_SOURCE_FILES} library.cpp library.h)
set(EXEC_SOURCE_FILES load_dll.cpp ${LIB_SOURCE_FILES} ${DLL_INJECT_LIB_SOURCE_FILES})

add_library(shared_dll SHARED ${DLL_SOURCE_FILES})

add_executable(test_dll ${EXEC_SOURCE_FILES})

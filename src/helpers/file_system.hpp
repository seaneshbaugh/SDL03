#ifndef SDL03_Helpers_FileSystem
#define SDL03_Helpers_FileSystem

#ifdef __APPLE__
#include "apple_file_system.hpp"
#elif _WIN32
#include "windows_file_system.hpp"
#elif __linux
#error Linux is not yet supported.
#else
#error Unknown and unsupported platform.
#endif

#endif


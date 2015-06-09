#ifndef __SDL03__FileSystemHelpers__
#define __SDL03__FileSystemHelpers__

#ifdef __APPLE__

#include "AppleFileSystemHelpers.h"

namespace FileSystemHelpers = AppleFileSystemHelpers;

#elif _WIN32

#error Windows is not yet supported.

#elif __linux

#error Linux is not yet supported.

#else

#error Unknown and unsupported platform.

#endif

#endif

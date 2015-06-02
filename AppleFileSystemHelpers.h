#ifndef __SDL03__AppleFileSystemHelpers__
#define __SDL03__AppleFileSystemHelpers__

#ifdef __APPLE__

#include <CoreServices/CoreServices.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "StringHelpers.h"

namespace AppleFileSystemHelpers {
    std::string GetApplicationDataDirectory();
    bool FileExists(std::string path);
    bool FileIsReadable(std::string path);
    bool FileIsWriteable(std::string path);
    bool IsDirectory(std::string path);
    bool IsFile(std::string path);
    bool CreateDirectory(std::string path, mode_t mode, bool recursive);
}

#endif

#endif


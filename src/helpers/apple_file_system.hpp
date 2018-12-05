#ifndef SDL03_Helpers_AppleFileSystem
#define SDL03_Helpers_AppleFileSystem

#ifdef __APPLE__

#include <math.h>
#include <CoreServices/CoreServices.h>

#include <pwd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "string.hpp"

namespace Helpers {
    namespace FileSystem {
        // TODO: Consider moving directory finding functions to their own namespace.
        // See: https://github.com/sago007/PlatformFolders
        std::string GetHomeDirectory();
        std::string GetLogDirectory();
        std::string GetApplicationDataDirectory();
        bool FileExists(std::string path);
        bool FileIsReadable(std::string path);
        bool FileIsWriteable(std::string path);
        bool IsDirectory(std::string path);
        bool IsFile(std::string path);
        bool CreateDirectory(std::string path, mode_t mode, bool recursive);
        bool ReadFile(std::string filename, std::string& contents);
    }
}

#endif

#endif


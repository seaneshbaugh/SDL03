#ifndef SDL03_Helpers_AppleFileSystem
#define SDL03_Helpers_AppleFileSystem

#ifdef WIN32

#include <shlobj.h>
#include <winerror.h>

// TODO: Use this for macOS file system stuff.
#include "../../lib/sago/platform_folders.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef int mode_t;

namespace Helpers
{
    namespace FileSystem
    {
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

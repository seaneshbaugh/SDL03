#ifdef __APPLE__

#include "AppleFileSystemHelpers.h"

// Apparently FSFindFolder and FSRefMakePath are depricated as of OSX 10.8.
std::string AppleFileSystemHelpers::GetApplicationDataDirectory() {
    FSRef ref;

    OSType folderType = kApplicationSupportFolderType;

    char path[PATH_MAX];

    FSFindFolder(kUserDomain, folderType, kCreateFolder, &ref);

    FSRefMakePath(&ref, (UInt8*)&path, PATH_MAX);

    return std::string(path);
}

bool AppleFileSystemHelpers::FileExists(std::string path) {
    return access(path.c_str(), F_OK) != -1;
}

bool AppleFileSystemHelpers::FileIsReadable(std::string path) {
    return access(path.c_str(), R_OK) != -1;
}

bool AppleFileSystemHelpers::FileIsWriteable(std::string path) {
    return access(path.c_str(), W_OK) != -1;
}

bool AppleFileSystemHelpers::IsDirectory(std::string path) {
    struct stat fileInfo;

    if (stat(path.c_str(), &fileInfo) != 0) {
        return false;
    } else {
        return fileInfo.st_mode & S_IFDIR;
    }
}

// This is probably prone to all sorts of weird edge case bugs.
bool AppleFileSystemHelpers::IsFile(std::string path) {
    return FileExists(path) && !IsDirectory(path);
}

// Based on http://stackoverflow.com/a/29828907/329602
bool AppleFileSystemHelpers::CreateDirectory(std::string path, mode_t mode = 0755, bool recursive = false) {
    int result = mkdir(path.c_str(), mode);

    if (result == 0) {
        return true;
    }

    switch (errno) {
        case ENOENT:
        {
            std::string::size_type pathDelimiterPosition = path.rfind("/");

            if (pathDelimiterPosition == std::string::npos) {
                return false;
            } else {
                if (recursive && CreateDirectory(path.substr(0, pathDelimiterPosition), mode, true)) {
                    return mkdir(path.c_str(), mode) == 0;
                } else {
                    return false;
                }
            }
        }
        case EEXIST:
        {
            return IsDirectory(path);
        }
        default:
        {
            return false;
        }
    }
}

#endif

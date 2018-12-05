#ifdef __APPLE__

#include "apple_file_system.hpp"

namespace Helpers {
    namespace FileSystem {
        std::string GetHomeDirectory() {
            uid_t uid = getuid();
            const char* home = std::getenv("HOME");

            if (uid != 0 && home) {
                return std::string(home);
            }

            struct passwd* pw = getpwuid(uid);

            if (!pw) {
                throw std::runtime_error("Unable to get passwd struct.");
            }

            const char* result = pw->pw_dir;

            if (!result) {
                throw std::runtime_error("User has no home directory");
            }

            return std::string(result);
        }

        std::string GetLogDirectory() {
            return GetHomeDirectory() + "/Library/Logs";
        }

        // FSFindFolder and FSRefMakePath are depricated as of OSX 10.8.
        std::string GetApplicationDataDirectory() {
            FSRef ref;
            OSType folderType = kApplicationSupportFolderType;
            char path[PATH_MAX];

            FSFindFolder(kUserDomain, folderType, kCreateFolder, &ref);

            FSRefMakePath(&ref, (UInt8*)&path, PATH_MAX);

            return std::string(path);
        }

        bool FileExists(std::string path) {
            return access(path.c_str(), F_OK) != -1;
        }

        bool FileIsReadable(std::string path) {
            return access(path.c_str(), R_OK) != -1;
        }

        bool FileIsWriteable(std::string path) {
            return access(path.c_str(), W_OK) != -1;
        }

        bool IsDirectory(std::string path) {
            struct stat fileInfo;

            if (stat(path.c_str(), &fileInfo) != 0) {
                return false;
            } else {
                return fileInfo.st_mode & S_IFDIR;
            }
        }

        bool IsFile(std::string path) {
            struct stat fileInfo;

            if (stat(path.c_str(), &fileInfo) != 0) {
                return false;
            } else {
                return fileInfo.st_mode & S_IFREG;
            }
        }

        // Based on http://stackoverflow.com/a/29828907/329602
        bool CreateDirectory(std::string path, mode_t mode = 0755, bool recursive = false) {
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

        // TODO: This is probably not very good.
        bool ReadFile(std::string filename, std::string &contents) {
            std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

            try {
                if (file) {
                    file.seekg(0, std::ios::end);

                    contents.resize(file.tellg());

                    file.seekg(0, std::ios::beg);

                    file.read(&contents[0], contents.size());

                    file.close();

                    return true;
                } else {
                    return false;
                }
            } catch (int exception) {
                return false;
            }
        }
    }
}

#endif

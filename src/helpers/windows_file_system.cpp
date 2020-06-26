#ifdef _WIN32

#include "windows_file_system.hpp"

namespace Helpers
{
    namespace FileSystem
    {
        std::wstring stringTowstring(const std::string& s) {
            int len;
            int slength = (int)s.length() + 1;
            len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
            wchar_t* buf = new wchar_t[len];
            MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
            std::wstring r(buf);
            delete[] buf;
            return r;
        }

        std::string GetHomeDirectory() {
            LPWSTR wszPath = NULL;
            HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile, KF_FLAG_CREATE, NULL, &wszPath);

            if (!SUCCEEDED(hr)) {
                CoTaskMemFree(wszPath);

                throw std::runtime_error("Profile could not be found");
            }

            CoTaskMemFree(wszPath);

            return sago::internal::win32_utf16_to_utf8(wszPath);
        }

        std::string GetLogDirectory() {
            return sago::getDataHome();
        }

        std::string GetApplicationDataDirectory() {
            return sago::getDataHome();
        }

        bool FileExists(std::string path) {
            return std::filesystem::exists(std::filesystem::status(path));
        }

        bool FileIsReadable(std::string path) {
            return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
        }

        bool FileIsWriteable(std::string path) {
            return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
        }

        bool IsDirectory(std::string path) {
            return std::filesystem::is_directory(std::filesystem::status(path));
        }

        bool IsFile(std::string path) {
            return std::filesystem::is_regular_file(std::filesystem::status(path));
        }

        bool CreateDirectory(std::string path, mode_t mode = 0755, bool recursive = false) {
            if (recursive) {
                return std::filesystem::create_directories(path);
            } else {
                return std::filesystem::create_directory(path);
            }

        }

        // TODO: This is probably not very good.
        bool ReadFile(std::string filename, std::string& contents) {
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

#include "path.hpp"

namespace Helpers {
    namespace Path {
        std::string BaseName(const std::string& path, const bool removeExtension = false) {
            std::string basename = path.substr(path.find_last_of("/\\") + 1);

            if (removeExtension) {
                return RemoveExtension(basename);
            }

            return basename;
        }

        std::string RemoveExtension(const std::string& filename) {
            typename std::string::size_type const p(filename.find_last_of('.'));
            return p > 0 && p != std::string::npos ? filename.substr(0, p) : filename;
        }
    }
}

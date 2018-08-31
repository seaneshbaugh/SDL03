#include "PathHelpers.h"

std::string PathHelpers::BaseName(const std::string &path, bool removeExtension = false) {
    std::string basename = path.substr(path.find_last_of("/\\") + 1);

    if (removeExtension) {
        return RemoveExtension(basename);
    }

    return basename;
}

std::string PathHelpers::RemoveExtension(const std::string &filename) {
    typename std::string::size_type const p(filename.find_last_of('.'));
    return p > 0 && p != std::string::npos ? filename.substr(0, p) : filename;
}

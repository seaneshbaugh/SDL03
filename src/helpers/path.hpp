#ifndef SDL03_Helpers_Path
#define SDL03_Helpers_Path

#include <string>

namespace Helpers {
    namespace Path {
        std::string BaseName(const std::string& path, const bool removeExtension);
        std::string RemoveExtension(const std::string& filename);
    }
}

#endif

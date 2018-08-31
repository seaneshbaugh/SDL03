#ifndef __SDL03__PathHelpers__
#define __SDL03__PathHelpers__

#include <string>

// I should probably find a proper library that handles just this stuff.
namespace PathHelpers {
    std::string BaseName(const std::string& path, bool removeExtension);
    std::string RemoveExtension(const std::string &filename);
};

#endif

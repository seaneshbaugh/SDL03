#ifndef __SDL03__StringHelpers__
#define __SDL03__StringHelpers__

#include <string>
#include <vector>

namespace StringHelpers {
    std::vector<std::string> Split(const std::string &string, const std::string &delimiter);
}

#endif
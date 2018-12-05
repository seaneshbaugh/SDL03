#ifndef SDL03_Helpers_String
#define SDL03_Helpers_String

#include <string>
#include <vector>

namespace Helpers {
    namespace String {
        std::vector<std::string> Split(const std::string& string, const std::string& delimiter);
    }
}

#endif

#ifndef SDL03_Helpers_String
#define SDL03_Helpers_String

#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace Helpers {
    namespace String {
        std::string Join(const std::vector<std::string>& strings, const std::string& delimiter);
        std::vector<std::string> Split(const std::string& string, const std::string& delimiter);
    }
}

#endif

#include "string.hpp"

namespace Helpers {
    namespace String {
        std::string Join(const std::vector<std::string>& strings, const std::string& delimiter) {
            if (delimiter.empty()) {
                throw std::invalid_argument("delimiter cannot be empty.");
            }

            if (strings.empty()) {
                return "";
            }

            return std::accumulate(next(begin(strings)), end(strings), strings[0],
                                   [&delimiter](std::string result, const std::string& value) {
                                       return result + delimiter + value;
                                   });
        }

        std::vector<std::string> Split(const std::string& source, const std::string& delimiter) {
            if (delimiter.empty()) {
                throw std::invalid_argument("delimiter cannot be empty.");
            }

            std::vector<std::string> tokens;

            std::string::size_type offset = 0;

            while (true) {
                std::string::size_type matchPosition = source.find(delimiter, offset);

                if (matchPosition != std::string::npos) {
                    tokens.push_back(source.substr(offset, matchPosition - offset));

                    offset = matchPosition + delimiter.size();
                } else {
                    tokens.push_back(source.substr(offset));

                    break;
                }
            }

            return tokens;
        }
    }
}

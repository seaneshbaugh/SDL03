#include "string.hpp"

namespace Helpers {
    namespace String {
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

#ifndef SDL03_Game_Parsers_AssetListParser
#define SDL03_Game_Parsers_AssetListParser

#include <map>
#include <string>

#include "../../../lib/LoggerCpp/include/LoggerCpp/LoggerCpp.h"
#include "../../../lib/nlohmann/json.hpp"

#include "../services/locator.hpp"

using json = nlohmann::json;

// TODO: Make a separate class for asset lists and add this to it.
// All other parsers are now private classes within the class they
// parse. This is because the parser needs to know the full definition
// of the class. It also kinda makes more sense.
namespace Game {
    namespace Parsers {
        class AssetListParser {
        public:
            AssetListParser();
            ~AssetListParser();
            std::map<std::string, std::string> Parse(const std::string& json);
        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
        };
    }
}

#endif

#include "asset_list_parser.hpp"

namespace Game {
    namespace Parsers {
        const std::string AssetListParser::logChannel = "json";

        AssetListParser::AssetListParser() {
            this->logger = Services::Locator::LoggerService()->GetLogger(AssetListParser::logChannel);
        }

        AssetListParser::~AssetListParser() {
        }

        std::map<std::string, std::string> AssetListParser::Parse(const std::string& jsonString) {
            std::map<std::string, std::string> assetList;

            // TODO: Consider using json flatten() here combined with a string find
            // and replace on the keys. Leaving this as is for now since this works
            // how I want it to.
            // See: https://github.com/nlohmann/json/blob/master/doc/examples/flatten.cpp
            std::function<void (const json&, const std::string&)> parseJSON = [&] (const json& node, const std::string& parentName) {
                for (auto it = node.begin(); it != node.end(); ++it) {
                    std::string name;

                    if (node.is_object()) {
                        if (parentName == "") {
                            name = it.key();
                        } else {
                            name = parentName + "." + it.key();
                        }
                    }

                    if ((*it).is_array() || (*it).is_object()) {
                        parseJSON(*it, name);
                    }

                    if ((*it).is_string() || (*it).is_number() || (*it).is_boolean()) {
                        std::string value = (*it).get<std::string>();

                        this->logger->debug() << name << " => " << value;

                        assetList[name] = value;
                    }
                }
            };

            json assetListNode = json::parse(jsonString);

            parseJSON(assetListNode, "");

            return assetList;
        }
    }
}

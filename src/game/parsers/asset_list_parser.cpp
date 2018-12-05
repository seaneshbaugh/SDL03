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
            JSONNode assetListNode = libjson::parse(jsonString);

            std::function<void(const JSONNode&, std::string)> parseJSON = [&] (const JSONNode& node, std::string parentName) {
                for (auto it = node.begin(); it != node.end(); ++it) {
                    std::string name;

                    if (parentName == "") {
                        name = it->name();
                    } else {
                        name = parentName + "." + it->name();
                    }

                    if (it->type() == JSON_ARRAY || it->type() == JSON_NODE) {
                        parseJSON(*it, name);
                    }

                    if (it->type() == JSON_STRING || it->type() == JSON_NUMBER || it->type() == JSON_BOOL) {
                        this->logger->debug() << name << " => " << it->as_string();

                        assetList[name] = it->as_string();
                    }
                }
            };

            parseJSON(assetListNode, "");

            return assetList;
        }
    }
}

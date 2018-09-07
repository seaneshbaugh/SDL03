#include "AssetListParser.h"

AssetListParser::AssetListParser() {
    this->logger = new Log::Logger("json");
}

AssetListParser::~AssetListParser() {
    delete this->logger;
}

bool AssetListParser::Parse(std::string json, std::map<std::string, std::string>* assetList) {
    JSONNode assetListNode = libjson::parse(json);
    
    std::function<void(const JSONNode&, std::string)> parseJSON = [&] (const JSONNode &node, std::string parentName) {
        JSONNode::const_iterator i = node.begin();
        
        while (i != node.end()) {
            std::string name;
            if (parentName == "") {
                name = i->name();
            } else {
                name = parentName + "." + i->name();
            }

            if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
                parseJSON(*i, name);
            }

            if (i->type() == JSON_STRING) {
                this->logger->debug() << name << " => " << i->as_string();

                (*assetList)[name] = i->as_string();
            }
            
            i++;
        }
    };
    
    parseJSON(assetListNode, "");
    
    return true;
}

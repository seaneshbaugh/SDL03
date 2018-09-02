#ifndef __SDL03__AssetListParser__
#define __SDL03__AssetListParser__

#include <map>

#include "LoggerCpp.h"
#include "../../../lib/libjson/libjson.h"

class AssetListParser {
public:
    Log::Logger* logger;

    AssetListParser();
    ~AssetListParser();
    bool Parse(std::string json, std::map<std::string, std::string> *assetList);
};

#endif

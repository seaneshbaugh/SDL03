#ifndef __SDL03__AssetListParser__
#define __SDL03__AssetListParser__

#include <map>

#include "../../../lib/libjson/libjson.h"

class AssetListParser {
public:
    bool Parse(std::string json, std::map<std::string, std::string> *assetList);
};

#endif

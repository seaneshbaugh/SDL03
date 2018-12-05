#ifndef SDL03_Game_Resources_ResourceList
#define SDL03_Game_Resources_ResourceList

#include <map>
#include <memory>
#include <string>

#include "libjson.h"

#include "../services/locator.hpp"

namespace Game {
    namespace Resources {
        template<class T>
        class ResourceList {
        public:
            ResourceList();
            ResourceList(const std::string& filename);
            ~ResourceList();
            std::map<std::string, T> LoadResources();

        private:
            std::string filename;
            std::map<std::string, std::string> assets;

            class Parser {
                Parser();
                ~Parser();
                std::map<std::string, std::string> Parse(const std::string& jsonString);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
            };
        };
    }
}

#endif

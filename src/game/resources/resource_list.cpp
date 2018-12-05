#include "resource_list.hpp"

namespace Game {
    namespace Resources {
        template<class T>ResourceList<T>::ResourceList() {
        }

        template<class T>ResourceList<T>::~ResourceList() {
        }

        template<class T> std::map<std::string, T> ResourceList<T>::LoadResources() {
        }

        template<class T> const std::string ResourceList<T>::Parser::Parser::logChannel = "json";

        template<class T>ResourceList<T>::Parser::Parser() {
            this->logger = Services::Locator::LoggerService()->GetLogger(ResourceList<T>::Parser::Parser::logChannel);
        }

        template<class T>ResourceList<T>::Parser::~Parser() {
        }

        template<class T> std::map<std::string, std::string> ResourceList<T>::Parser::Parse(const std::string& jsonString) {
            std::map<std::string, std::string> resourceList;
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

                    // This might be a good place for std::variant
                    if (it->type() == JSON_STRING || it->type() == JSON_NUMBER || it->type() == JSON_BOOL) {
                        this->logger->debug() << name << " => " << it->as_string();

                        resourceList[name] = it->as_string();
                    }
                }
            };

            parseJSON(assetListNode, "");

            return resourceList;
        }
    }
}

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

                        resourceList[name] = value;
                    }
                }
            };

            json resourceListNode = json::parse(jsonString);

            parseJSON(resourceListNode, "");

            return resourceList;
        }
    }
}

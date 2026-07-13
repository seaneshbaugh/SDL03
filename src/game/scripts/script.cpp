#include "script.hpp"

namespace Game {
    namespace Scripts {
        Script::Script(Scenes::Scene* scene, Interfaces::DialogueManager* dialogueManager, const std::string& scriptId) : scene(scene), dialogueManager(dialogueManager), scriptId(scriptId) {
            this->Load(scriptId);
        }

        Script::~Script() {
        }

        bool Script::Load(const std::string& scriptId) {
            this->scriptId = scriptId;

            std::string jsonString;

            if (!Helpers::FileSystem::ReadFile("data/scripts/" + scriptId + ".json", jsonString)) {
                return false;
            }

            if (!this->ParseFile(jsonString)) {
                return false;
            }

            return true;
        }

        bool Script::ParseFile(const std::string& jsonString) {
            Script::Parser parser;
            
            parser.Parse(jsonString, this);

            return true;
        }

        const std::string Script::Parser::logChannel = "json";

        Script::Parser::Parser() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Script::Parser::logChannel);
        }

        Script::Parser::~Parser() {
        }

        void Script::Parser::Parse(const std::string& jsonString, Script* script) {
            json scriptJson = json::parse(jsonString);

            std::string startNodeKey = scriptJson["start"].get<std::string>();

            json nodesNode = scriptJson["nodes"];

            ScriptNode::Parser nodeParser;

            for (json::iterator nodeNode = nodesNode.begin(); nodeNode != nodesNode.end(); ++nodeNode) {
                script->nodes[nodeNode.key()] = nodeParser.ParseNode(nodeNode.value(), script);
            }

            for (json::iterator nodeNode = nodesNode.begin(); nodeNode != nodesNode.end(); ++nodeNode) {
                std::string nodeId = nodeNode.key();
                json nodeData = nodeNode.value();

                if (nodeData.find("next") != nodeData.end() && !nodeData["next"].is_null()) {
                    std::string nextNodeKey = nodeData["next"].get<std::string>();

                    if (script->nodes.find(nextNodeKey) == script->nodes.end()) {
                        this->logger->error() << "Invalid next node key \"" << nextNodeKey << "\" for node with key \"" << nodeId << "\".";

                        continue;
                    }

                    script->nodes[nodeId]->next = script->nodes[nextNodeKey];
                }
            }

            if (script->nodes.find(startNodeKey) == script->nodes.end()) {
                this->logger->error() << "Invalid start node key \"" << startNodeKey << "\".";

                return;
            } else {
                script->root = script->nodes[startNodeKey];
            }
        }
    }
}

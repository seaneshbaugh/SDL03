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

            if (!Helpers::FileSystem::ReadFile("data/cutscenes/" + scriptId + ".json", jsonString)) {
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

            json actionsNode = scriptJson["actions"];

            Actions::ActionParser actionParser;

            for (json::iterator actionNode = actionsNode.begin(); actionNode != actionsNode.end(); ++actionNode) {
                std::shared_ptr<Actions::Base> action = actionParser.ParseAction(*actionNode, script);

                if (action) {
                    script->actions.push_back(action);
                }
            }
        }
    }
}

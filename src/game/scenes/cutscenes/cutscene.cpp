#include "cutscene.hpp"
//#include "../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            Cutscene::Cutscene(Scenes::Scene* scene, Interfaces::DialogueManager* dialogueManager, const std::string& cutsceneId) : scene(scene), dialogueManager(dialogueManager), cutsceneId(cutsceneId) {
                this->Load(cutsceneId);
            }

            Cutscene::~Cutscene() {
            }

            bool Cutscene::Load(const std::string& cutsceneId) {
                this->cutsceneId = cutsceneId;

                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile("data/cutscenes/" + cutsceneId + ".json", jsonString)) {
                    return false;
                }

                if (!this->ParseFile(jsonString)) {
                    return false;
                }

                return true;
            }

            bool Cutscene::ParseFile(const std::string& jsonString) {
                std::unique_ptr<Cutscene::Parser> parser = std::make_unique<Cutscene::Parser>();

                parser->Parse(jsonString, this);

                return true;
            }

            const std::string Cutscene::Parser::logChannel = "json";

            Cutscene::Parser::Parser() {
                this->logger = Services::Locator::LoggerService()->GetLogger(Cutscene::Parser::logChannel);
            }

            Cutscene::Parser::~Parser() {
            }

            void Cutscene::Parser::Parse(const std::string& jsonString, Cutscene* cutscene) {
                //json cutsceneJson = json::parse(jsonString);

                //json actionsNode = cutsceneJson["actions"];

                //Actions::ActionParser actionParser;

                //for (json::iterator actionNode = actionsNode.begin(); actionNode != actionsNode.end(); ++actionNode) {
                //    std::shared_ptr<Actions::Base> action = actionParser.ParseAction(*actionNode, cutscene);

                //    if (action) {
                //        cutscene->actions.push_back(action);
                //    }
                //}
            }
        }
    }
}

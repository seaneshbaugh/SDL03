#include "cutscene.hpp"
#include "../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            Cutscene::Cutscene(States::Map* map, const std::string& cutsceneId) : map(map), cutsceneId(cutsceneId) {
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
                json cutsceneJson = json::parse(jsonString);

                json actionsNode = cutsceneJson["actions"];

                for (json::iterator actionNode = actionsNode.begin(); actionNode != actionsNode.end(); ++actionNode) {
                    std::string actionType = actionNode.value()["type"].get<std::string>();

                    if (actionType == "add_actor") {
                        std::string id = actionNode.value()["id"].get<std::string>();
                        std::string name = actionNode.value()["name"].get<std::string>();
                        std::string spritesheetName = actionNode.value()["spritesheetName"].get<std::string>();
                        std::string dialogueId = actionNode.value()["dialogueId"].get<std::string>();
                        Scene::Actor::Direction direction = static_cast<Scene::Actor::Direction>(actionNode.value()["direction"].get<int>());
                        std::string movementScriptName = actionNode.value()["movementScriptName"].get<std::string>();
                        std::string interactionScriptName = actionNode.value()["interactionScriptName"].get<std::string>();

                        int x = 0;
                        int y = 0;

                        if (actionNode.value()["spawnPointId"].is_string()) {
                            std::string spawnPointId = actionNode.value()["spawnPointId"].get<std::string>();
                            std::shared_ptr<Objects::Maps::SpawnPoint> spawnPoint = cutscene->map->GetCurrentMap()->GetSpawnPoint(spawnPointId);

                            x = spawnPoint->x;
                            y = spawnPoint->y;
                        } else {
                            x = actionNode.value()["x"].get<int>();
                            y = actionNode.value()["y"].get<int>();
                        }

                        std::shared_ptr<Actions::AddActor> addActorAction = std::make_shared<Actions::AddActor>(cutscene->map, id, name, spritesheetName, dialogueId, x, y, direction, movementScriptName, interactionScriptName);

                        cutscene->actions.push_back(addActorAction);
                    } else if (actionType == "dialogue") {
                        std::string dialogueId = actionNode.value()["dialogueId"].get<std::string>();

                        std::shared_ptr<Actions::Dialogue> dialogueAction = std::make_shared<Actions::Dialogue>(cutscene->map, dialogueId);

                        cutscene->actions.push_back(dialogueAction);
                    } else if (actionType == "move_actor") {
                        std::string actorId = actionNode.value()["actorId"].get<std::string>();

                        std::vector<std::string> rawPath = actionNode.value()["path"].get<std::vector<std::string>>();

                        std::vector<Scene::Actor::Direction> path;

                        for (const std::string& rawDirection : rawPath) {
                            if (rawDirection == "up") {
                                path.push_back(Scene::Actor::Direction::Up);
                            } else if (rawDirection == "right") {
                                path.push_back(Scene::Actor::Direction::Right);
                            } else if (rawDirection == "down") {
                                path.push_back(Scene::Actor::Direction::Down);
                            } else if (rawDirection == "left") {
                                path.push_back(Scene::Actor::Direction::Left);
                            } else {
                                this->logger->warning() << "Unknown direction \"" << rawDirection << "\" in move_actor action.";
                            }
                        }

                        std::shared_ptr<Actions::MoveActor> moveActorAction = std::make_shared<Actions::MoveActor>(cutscene->map, actorId, path);

                        cutscene->actions.push_back(moveActorAction);
                    } else if (actionType == "wait") {
                        float duration = actionNode.value()["duration"].get<float>();

                        std::shared_ptr<Actions::Wait> waitAction = std::make_shared<Actions::Wait>(duration);

                        cutscene->actions.push_back(waitAction);
                    } else {
                        this->logger->warning() << "Unknown cutscene action type \"" << actionType << "\".";
                    }
                }
            }
        }
    }
}

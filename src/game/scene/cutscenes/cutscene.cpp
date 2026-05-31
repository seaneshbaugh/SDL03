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

                this->actionFactories["add_actor"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::string id = node["id"].get<std::string>();
                    std::string name = node["name"].get<std::string>();
                    std::string spritesheetName = node["spritesheetName"].get<std::string>();
                    std::string dialogueId = node["dialogueId"].get<std::string>();
                    Scene::Actor::Direction direction = static_cast<Scene::Actor::Direction>(node["direction"].get<int>());
                    std::string movementScriptName = node["movementScriptName"].get<std::string>();
                    std::string interactionScriptName = node["interactionScriptName"].get<std::string>();

                    int x = 0;
                    int y = 0;

                    if (node["spawnPointId"].is_string()) {
                        std::string spawnPointId = node["spawnPointId"].get<std::string>();
                        std::shared_ptr<Objects::Maps::SpawnPoint> spawnPoint = cutscene->map->GetCurrentMap()->GetSpawnPoint(spawnPointId);

                        x = spawnPoint->x;
                        y = spawnPoint->y;
                    } else {
                        x = node["x"].get<int>();
                        y = node["y"].get<int>();
                    }

                    return std::make_shared<Actions::AddActor>(cutscene->map, id, name, spritesheetName, dialogueId, x, y, direction, movementScriptName, interactionScriptName);
                };

                this->actionFactories["dialogue"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::string dialogueId = node["dialogueId"].get<std::string>();

                    return std::make_shared<Actions::Dialogue>(cutscene->map, dialogueId);
                };

                this->actionFactories["move_actor"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::string actorId = node["actorId"].get<std::string>();

                    std::vector<std::string> rawPath = node["path"].get<std::vector<std::string>>();
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

                    return std::make_shared<Actions::MoveActor>(cutscene->map, actorId, path);
                };

                this->actionFactories["parallel"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::vector<std::shared_ptr<Actions::Base>> actions;

                    for (const json& child : node["actions"]) {
                        std::shared_ptr<Actions::Base> action = this->ParseAction(child, cutscene);

                        if (action) {
                            actions.push_back(action);
                        }
                    }

                    return std::make_shared<Actions::Parallel>(actions);
                };

                this->actionFactories["pathfind_actor"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::string actorId = node["actorId"].get<std::string>();

                    if (node["targetId"].is_string()) {
                        std::string targetId = node["targetId"].get<std::string>();

                        return std::make_shared<Actions::PathfindActor>(cutscene->map, actorId, targetId);
                    } else {
                        int x = node["x"].get<int>();
                        int y = node["y"].get<int>();

                        return std::make_shared<Actions::PathfindActor>(cutscene->map, actorId, x, y);
                    }
                };

                this->actionFactories["remove_actor"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::string actorId = node["actorId"].get<std::string>();

                    return std::make_shared<Actions::RemoveActor>(cutscene->map, actorId);
                };

                this->actionFactories["sequence"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    std::vector<std::shared_ptr<Actions::Base>> actions;

                    for (const json& child : node["actions"]) {
                        std::shared_ptr<Actions::Base> action = this->ParseAction(child, cutscene);

                        if (action) {
                            actions.push_back(action);
                        }
                    }

                    return std::make_shared<Actions::Sequence>(actions);
                };

                this->actionFactories["wait"] = [this](const json& node, Cutscene* cutscene) -> std::shared_ptr<Actions::Base> {
                    float duration = node["duration"].get<float>();

                    return std::make_shared<Actions::Wait>(duration);
                };
            }

            Cutscene::Parser::~Parser() {
            }

            void Cutscene::Parser::Parse(const std::string& jsonString, Cutscene* cutscene) {
                json cutsceneJson = json::parse(jsonString);

                json actionsNode = cutsceneJson["actions"];

                for (json::iterator actionNode = actionsNode.begin(); actionNode != actionsNode.end(); ++actionNode) {
                    std::shared_ptr<Actions::Base> action = this->ParseAction(*actionNode, cutscene);

                    if (action) {
                        cutscene->actions.push_back(action);
                    }
                }
            }

            std::shared_ptr<Actions::Base> Cutscene::Parser::ParseAction(const json& node, Cutscene* cutscene) {
                std::string actionType = node["type"].get<std::string>();

                auto actionFactory = this->actionFactories.find(actionType);

                if (actionFactory != this->actionFactories.end()) {
                    return actionFactory->second(node, cutscene);
                } else {
                    this->logger->warning() << "Unknown cutscene action type \"" << actionType << "\".";

                    return nullptr;
                }
            }
        }
    }
}

#include "action_parser.hpp"
#include "../scripts/script.hpp"

namespace Game {
    namespace Actions {
        const std::string ActionParser::logChannel = "json";

        ActionParser::ActionParser() {
            this->logger = Services::Locator::LoggerService()->GetLogger(ActionParser::logChannel);

            this->actionFactories["add_actor"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string id = node["id"].get<std::string>();
                std::string name = node["name"].get<std::string>();
                std::string spritesheetName = node["spritesheetName"].get<std::string>();
                std::string dialogueId = node["dialogueId"].get<std::string>();
                Direction direction = static_cast<Direction>(node["direction"].get<int>());
                std::string movementScriptName = node["movementScriptName"].get<std::string>();
                std::string interactionScriptName = node["interactionScriptName"].get<std::string>();

                int x = 0;
                int y = 0;

                if (node.find("spawnPointId") != node.end() && node["spawnPointId"].is_string()) {
                    std::string spawnPointId = node["spawnPointId"].get<std::string>();
                    std::shared_ptr<Objects::Maps::SpawnPoint> spawnPoint = script->scene->GetCurrentMap()->GetSpawnPoint(spawnPointId);

                    x = spawnPoint->x;
                    y = spawnPoint->y;
                } else {
                    x = node["x"].get<int>();
                    y = node["y"].get<int>();
                }

                return std::make_shared<Actions::AddActor>(script->scene, id, name, spritesheetName, dialogueId, x, y, direction, movementScriptName, interactionScriptName);
            };

            this->actionFactories["animate_actor"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string actorId = node["actorId"].get<std::string>();
                std::string animationName = node["animation"].get<std::string>();
                int loops = node["loops"].get<int>();
                float duration = node["duration"].get<float>();

                return std::make_shared<Actions::AnimateActor>(script->scene, actorId, animationName, loops, duration);
            };

            this->actionFactories["face_actor"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string actorId = node["actorId"].get<std::string>();

                if (node.find("targetId") != node.end() && node["targetId"].is_string()) {
                    std::string targetId = node["targetId"].get<std::string>();

                    return std::make_shared<Actions::FaceActor>(script->scene, actorId, targetId);
                } else {
                    Direction direction = static_cast<Direction>(node["direction"].get<int>());

                    return std::make_shared<Actions::FaceActor>(script->scene, actorId, direction);
                }
            };

            this->actionFactories["move_actor"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string actorId = node["actorId"].get<std::string>();

                std::vector<std::string> rawPath = node["path"].get<std::vector<std::string>>();
                std::vector<Direction> path;

                for (const std::string& rawDirection : rawPath) {
                    if (rawDirection == "up") {
                        path.push_back(Direction::Up);
                    } else if (rawDirection == "right") {
                        path.push_back(Direction::Right);
                    } else if (rawDirection == "down") {
                        path.push_back(Direction::Down);
                    } else if (rawDirection == "left") {
                        path.push_back(Direction::Left);
                    } else {
                        this->logger->warning() << "Unknown direction \"" << rawDirection << "\" in move_actor action.";
                    }
                }

                return std::make_shared<Actions::MoveActor>(script->scene, actorId, path);
            };

            this->actionFactories["parallel"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::vector<std::shared_ptr<Actions::Base>> actions;

                for (const json& child : node["actions"]) {
                    std::shared_ptr<Actions::Base> action = this->ParseAction(child, script);

                    if (action) {
                        actions.push_back(action);
                    }
                }

                return std::make_shared<Actions::Parallel>(actions);
            };

            this->actionFactories["pathfind_actor"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string actorId = node["actorId"].get<std::string>();

                if (node.find("targetId") != node.end() && node["targetId"].is_string()) {
                    std::string targetId = node["targetId"].get<std::string>();

                    return std::make_shared<Actions::PathfindActor>(script->scene, actorId, targetId);
                } else {
                    int x = node["x"].get<int>();
                    int y = node["y"].get<int>();

                    return std::make_shared<Actions::PathfindActor>(script->scene, actorId, x, y);
                }
            };

            this->actionFactories["remove_actor"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string actorId = node["actorId"].get<std::string>();

                return std::make_shared<Actions::RemoveActor>(script->scene, actorId);
            };

            this->actionFactories["sequence"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::vector<std::shared_ptr<Actions::Base>> actions;

                for (const json& child : node["actions"]) {
                    std::shared_ptr<Actions::Base> action = this->ParseAction(child, script);

                    if (action) {
                        actions.push_back(action);
                    }
                }

                return std::make_shared<Actions::Sequence>(actions);
            };

            this->actionFactories["set_flag"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                std::string key = node["key"].get<std::string>();
                bool value = node["value"].get<bool>();

                return std::make_shared<Actions::SetFlag>(key, value);
            };

            this->actionFactories["wait"] = [this](const json& node, Scripts::Script* script) -> std::shared_ptr<Actions::Base> {
                float duration = node["duration"].get<float>();

                return std::make_shared<Actions::Wait>(duration);
            };
        }

        ActionParser::~ActionParser() {
        }

        std::shared_ptr<Actions::Base> ActionParser::ParseAction(const json& node, Scripts::Script* script) {
            std::string actionType = node["type"].get<std::string>();

            auto actionFactory = this->actionFactories.find(actionType);

            if (actionFactory != this->actionFactories.end()) {
                return actionFactory->second(node, script);
            } else {
                this->logger->warning() << "Unknown action type \"" << actionType << "\".";

                return nullptr;
            }
        }
    }
}

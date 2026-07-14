#include "script_node.hpp"
#include "script.hpp"

namespace Game {
    namespace Scripts {
        ScriptNode::ScriptNode() : next(nullptr) {
        }

        ScriptNode::~ScriptNode() {
        }

        const std::string ScriptNode::Parser::logChannel = "json";

        ScriptNode::Parser::Parser() {
            this->logger = Services::Locator::LoggerService()->GetLogger(ScriptNode::Parser::logChannel);

            this->scriptStepFactories["action"] = [this](const json& node, Script* script) -> std::shared_ptr<Steps::ScriptStep> {
                if (node.find("action") == node.end() || !node["action"].is_object()) {
                    this->logger->error() << "Missing or invalid \"action\" object for action step.";

                    return nullptr;
                }

                std::shared_ptr<Steps::ActionStep> actionStep = std::make_shared<Steps::ActionStep>();

                Actions::ActionParser actionParser;

                actionStep->action = actionParser.ParseAction(node["action"], script);

                return actionStep;
            };

            this->scriptStepFactories["choice"] = [this](const json& node, Script* script) -> std::shared_ptr<Steps::ScriptStep> {
                if (node.find("actorId") == node.end() || !(node["actorId"].is_string() || node["actorId"].is_null())) {
                    this->logger->error() << "Missing or invalid \"actorId\" for choice step.";

                    return nullptr;
                }

                if (node.find("text") == node.end() || !node["text"].is_string()) {
                    this->logger->error() << "Missing or invalid \"text\" for choice step.";

                    return nullptr;
                }

                if (node.find("choices") == node.end() || !node["choices"].is_array()) {
                    this->logger->error() << "Missing or invalid \"choices\" for choice step.";

                    return nullptr;
                }

                for (const auto& choiceNode : node["choices"]) {
                    if (!choiceNode.is_object()) {
                        this->logger->error() << "Choice is not an object for choice step.";

                        return nullptr;
                    }
                }

                std::shared_ptr<Steps::ChoiceStep> choiceStep = std::make_shared<Steps::ChoiceStep>();
                std::unique_ptr<Dialogue::DialoguePage> dialoguePage = std::make_unique<Dialogue::DialoguePage>();

                choiceStep->page = std::move(dialoguePage);

                choiceStep->actorId = node["actorId"].is_null() ? std::nullopt : std::optional<std::string>(node["actorId"].get<std::string>());
                choiceStep->page->text = node["text"].get<std::string>();

                for (const auto& choiceNode : node["choices"]) {
                    if (choiceNode.find("text") == choiceNode.end() || !choiceNode["text"].is_string()) {
                        this->logger->error() << "Missing or invalid \"text\" for choice in choice step.";

                        return nullptr;
                    }

                    if (choiceNode.find("next") == choiceNode.end() || !choiceNode["next"].is_string()) {
                        this->logger->error() << "Missing or invalid \"next\" for choice in choice step.";

                        return nullptr;
                    }

                    std::string text = choiceNode["text"].get<std::string>();
                    std::string nextNodeId = choiceNode["next"].get<std::string>();

                    choiceStep->page->choices.push_back(Dialogue::DialogueChoice{text, nextNodeId});
                }

                return choiceStep;
            };

            this->scriptStepFactories["dialogue"] = [this](const json& node, Script* script) -> std::shared_ptr<Steps::ScriptStep> {
                if (node.find("actorId") == node.end() || !(node["actorId"].is_string() || node["actorId"].is_null())) {
                    this->logger->error() << "Missing or invalid \"actorId\" for dialogue step.";

                    return nullptr;
                }

                if (node.find("text") == node.end() || !node["text"].is_string()) {
                    this->logger->error() << "Missing or invalid \"text\" for dialogue step.";

                    return nullptr;
                }

                std::shared_ptr<Steps::DialogueStep> dialogueStep = std::make_shared<Steps::DialogueStep>();
                std::unique_ptr<Dialogue::DialoguePage> dialoguePage = std::make_unique<Dialogue::DialoguePage>();

                dialogueStep->page = std::move(dialoguePage);

                dialogueStep->actorId = node["actorId"].is_null() ? std::nullopt : std::optional<std::string>(node["actorId"].get<std::string>());
                dialogueStep->page->text = node["text"].get<std::string>();

                return dialogueStep;
            };
        }

        ScriptNode::Parser::~Parser() {
        }

        std::shared_ptr<ScriptNode> ScriptNode::Parser::ParseNode(const json& node, Script* script) {
            std::shared_ptr<ScriptNode> scriptNode = std::make_shared<ScriptNode>();

            for (const auto& stepNode : node["steps"]) {
                if (!stepNode.is_object()) {
                    this->logger->error() << "Step is not an object for node.";
                    continue;
                }

                if (stepNode.find("type") == stepNode.end() || !stepNode["type"].is_string()) {
                    this->logger->error() << "Missing or invalid \"type\" for step in node.";
                    continue;
                }

                std::string stepType = stepNode["type"].get<std::string>();

                if (this->scriptStepFactories.find(stepType) == this->scriptStepFactories.end()) {
                    this->logger->error() << "Invalid step type \"" << stepType << "\" for step in node.";

                    continue;
                }

                std::shared_ptr<Steps::ScriptStep> scriptStep = this->scriptStepFactories[stepType](stepNode, script);

                if (scriptStep) {
                    scriptNode->steps.push_back(scriptStep);
                }
            }

            return scriptNode;
        }
    }
}

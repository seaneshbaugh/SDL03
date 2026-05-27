#include "dialogue_graph.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            DialogueGraph::DialogueGraph(const std::string& graphName) : graphName(graphName), root(nullptr), current(nullptr) {
                this->Load(this->graphName);
            }

            DialogueGraph::~DialogueGraph() {
            }

            bool DialogueGraph::Load(const std::string& graphName) {
                this->graphName = graphName;

                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile("data/dialogue/graphs/" + graphName + ".json", jsonString)) {
                    return false;
                }

                if (!this->ParseFile(jsonString)) {
                    return false;
                }

                return true;
            }

            bool DialogueGraph::ParseFile(const std::string& jsonString) {
                std::unique_ptr<DialogueGraph::Parser> parser = std::make_unique<DialogueGraph::Parser>();

                parser->Parse(jsonString, this);

                return true;
            }

            const std::string DialogueGraph::Parser::logChannel = "json";

            DialogueGraph::Parser::Parser() {
                this->logger = Services::Locator::LoggerService()->GetLogger(DialogueGraph::Parser::logChannel);
            }

            DialogueGraph::Parser::~Parser() {
            }

            void DialogueGraph::Parser::Parse(const std::string& jsonString, DialogueGraph* graph) {
                json dialogueGraphNode = json::parse(jsonString);

                std::string startNodeKey = dialogueGraphNode["start"].get<std::string>();

                json dialogueNodesNode = dialogueGraphNode["nodes"];

                std::map<std::string, std::shared_ptr<DialogueNode>> dialogueNodes;

                for (json::iterator dialogueNodeNode = dialogueNodesNode.begin(); dialogueNodeNode != dialogueNodesNode.end(); ++dialogueNodeNode) {
                    std::string dialogueNodeKey = dialogueNodeNode.key();
                    json dialogueNodeData = dialogueNodeNode.value();
                    DialogueNode::Type dialogueNodeType;

                    if (dialogueNodeData["type"] == "text") {
                        dialogueNodeType = DialogueNode::Type::Text;
                    } else if (dialogueNodeData["type"] == "choice") {
                        dialogueNodeType = DialogueNode::Type::Choice;
                    } else {
                        this->logger->error() << "Invalid dialogue node type \"" << dialogueNodeData["type"] << "\" for node with key \"" << dialogueNodeKey << "\".";

                        continue;
                    }

                    std::string dialogueNodeText = dialogueNodeData["text"].get<std::string>();

                    std::shared_ptr<DialogueNode> dialogueNode = std::make_shared<DialogueNode>(dialogueNodeType, dialogueNodeKey, dialogueNodeText);

                    dialogueNodes[dialogueNodeKey] = dialogueNode;
                }

                for (json::iterator dialogueNodeNode = dialogueNodesNode.begin(); dialogueNodeNode != dialogueNodesNode.end(); ++dialogueNodeNode) {
                    std::string dialogueNodeKey = dialogueNodeNode.key();
                    json dialogueNodeData = dialogueNodeNode.value();

                    if (dialogueNodeData.find("next") != dialogueNodeData.end() && !dialogueNodeData["next"].is_null()) {
                        std::string nextDialogueNodeKey = dialogueNodeData["next"].get<std::string>();

                        if (dialogueNodes.find(nextDialogueNodeKey) == dialogueNodes.end()) {
                            this->logger->error() << "Invalid next node key \"" << nextDialogueNodeKey << "\" for node with key \"" << dialogueNodeKey << "\".";

                            continue;
                        }

                        dialogueNodes[dialogueNodeKey]->next = dialogueNodes[nextDialogueNodeKey];
                    }

                    if (dialogueNodeData.find("choices") != dialogueNodeData.end()) {
                        json choicesData = dialogueNodeData["choices"];

                        for (json::iterator choiceData = choicesData.begin(); choiceData != choicesData.end(); ++choiceData) {
                            std::string choiceText = choiceData.value()["text"].get<std::string>();
                            std::string nextDialogueNodeKey = choiceData.value()["next"].get<std::string>();

                            if (dialogueNodes.find(nextDialogueNodeKey) == dialogueNodes.end()) {
                                this->logger->error() << "Invalid next node key \"" << nextDialogueNodeKey << "\" for choice with text \"" << choiceText << "\" for node with key \"" << dialogueNodeKey << "\".";

                                continue;
                            }

                            DialogueChoice dialogueChoice(choiceText, dialogueNodes[nextDialogueNodeKey]);

                            dialogueNodes[dialogueNodeKey]->choices.push_back(dialogueChoice);
                        }
                    }
                }

                if (dialogueNodes.find(startNodeKey) == dialogueNodes.end()) {
                    this->logger->error() << "Invalid start node key \"" << startNodeKey << "\".";

                    return;
                } else {
                    graph->root = dialogueNodes[startNodeKey];
                    graph->current = graph->root;
                }
            }
        }
    }
}

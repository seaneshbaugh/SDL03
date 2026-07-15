#include "script_runner.hpp"

namespace Game {
    namespace Scripts {
        ScriptRunner::ScriptRunner() : currentScript(nullptr), currentNode(nullptr), pendingNode(nullptr), currentStep(nullptr), completed(false) {
        }

        ScriptRunner::~ScriptRunner() {
        }

        void ScriptRunner::Start(std::shared_ptr<Script> script) {
            this->currentScript = script;
            this->SetCurrentNode(script->root);
            this->completed = false;

            if (this->currentStep) {
                this->currentStep->Start(this);
            }
        }

        void ScriptRunner::Update(const float deltaTime) {
            if (this->completed || !this->currentStep) {
                return;
            }

            this->currentStep->Update(deltaTime);

            if (this->currentStep->IsCompleted()) {
                this->AdvanceToNextStep();
            }
        }

        void ScriptRunner::Render() {
            if (this->currentStep) {
                this->currentStep->Render();
            }
        }

        bool ScriptRunner::IsCompleted() const {
            return this->completed;
        }

        void ScriptRunner::AdvanceToNextStep() {
            this->steps.pop();

            if (this->steps.empty()) {
                if (this->pendingNode) {
                    this->SetCurrentNode(this->pendingNode);

                    this->pendingNode.reset();

                    if (this->currentStep) {
                        this->currentStep->Start(this);
                    }
                } else if (this->currentNode->next) {
                    this->SetCurrentNode(this->currentNode->next);

                    if (this->currentStep) {
                        this->currentStep->Start(this);
                    }
                } else {
                    this->completed = true;
                }
            } else {
                this->currentStep = this->steps.front();

                this->currentStep->Start(this);
            }
        }

        void ScriptRunner::JumpToNode(const std::string& nodeId) {
            this->pendingNode = this->currentScript->nodes[nodeId];
        }

        void ScriptRunner::SetCurrentNode(std::shared_ptr<ScriptNode> node) {
            this->currentNode = node;

            this->steps = std::queue<std::shared_ptr<Steps::ScriptStep>>();

            for (auto& step : node->steps) {
                this->steps.push(step);
            }

            if (!this->steps.empty()) {
                this->currentStep = this->steps.front();
            } else {
                this->currentStep = nullptr;
            }
        }
    }
}

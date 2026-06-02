#include "sequence.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                Sequence::Sequence(std::vector<std::shared_ptr<Base>> actions) : actions(actions), currentActionIndex(0) {
                }

                Sequence::~Sequence() {
                }

                void Sequence::Start() {
                    if (!this->actions.empty()) {
                        this->actions[0]->Start();
                    }
                }

                void Sequence::Update(float deltaTime) {
                    if (this->currentActionIndex < this->actions.size()) {
                        std::shared_ptr<Base> currentAction = this->actions[this->currentActionIndex];

                        currentAction->Update(deltaTime);

                        if (currentAction->IsCompleted()) {
                            this->currentActionIndex++;

                            if (this->currentActionIndex < this->actions.size()) {
                                this->actions[this->currentActionIndex]->Start();
                            }
                        }
                    }
                }

                bool Sequence::IsCompleted() const {
                    return this->currentActionIndex >= this->actions.size();
                }
            }
        }
    }
}

#include "parallel.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                Parallel::Parallel(std::vector<std::shared_ptr<Base>> actions) : actions(actions) {
                }

                Parallel::~Parallel() {
                }

                void Parallel::Start() {
                    for (auto& action : this->actions) {
                        action->Start();
                    }
                }

                void Parallel::Update(float deltaTime) {
                    for (auto& action : this->actions) {
                        action->Update(deltaTime);
                    }
                }

                bool Parallel::IsCompleted() const {
                    for (auto& action : this->actions) {
                        if (!action->IsCompleted()) {
                            return false;
                        }
                    }

                    return true;
                }
            }
        }
    }
}

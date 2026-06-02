#include "wait.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                Wait::Wait(float duration) : duration(duration), elapsed(0.0f) {
                }

                void Wait::Start() {
                    this->elapsed = 0.0f;
                }

                void Wait::Update(float deltaTime) {
                    this->elapsed += deltaTime;
                }

                bool Wait::IsCompleted() const {
                    return this->elapsed >= this->duration;
                }
            }
        }
    }
}

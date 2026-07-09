#include "action_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            ActionStep::ActionStep() {
            }

            ActionStep::~ActionStep() {
            }

            void ActionStep::Start() {
            }

            void ActionStep::Update(const float deltaTime) {
            }

            bool ActionStep::IsCompleted() const {
                return true;
            }
        }
    }
}

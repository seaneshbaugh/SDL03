#ifndef SDL03_Game_Actions_ActionRunner
#define SDL03_Game_Actions_ActionRunner

#include <memory>
#include <queue>
#include <vector>

#include "base.hpp"

namespace Game {
    namespace Actions {
        class ActionRunner {
        public:
            ActionRunner();
            ~ActionRunner();
            void Start(std::vector<std::shared_ptr<Actions::Base>> actions);
            void Update(const float deltaTime);
            bool IsCompleted() const;

        private:
            std::queue<std::shared_ptr<Actions::Base>> actions;
            std::shared_ptr<Actions::Base> currentAction;
            bool completed;
        };
    }
}

#endif

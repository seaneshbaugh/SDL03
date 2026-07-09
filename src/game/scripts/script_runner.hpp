#ifndef SDL03_Game_Scripts_ScriptRunner
#define SDL03_Game_Scripts_ScriptRunner

#include "../actions/action_runner.hpp"
#include "script.hpp"

namespace Game {
    namespace Scripts {
        class ScriptRunner {
        public:
            ScriptRunner();
            ~ScriptRunner();
            void Start(std::shared_ptr<Script> script);
            void Update(const float deltaTime);
            bool IsCompleted() const;

        private:
            std::shared_ptr<Script> currentScript;
            Actions::ActionRunner actionRunner;
        };
    }
}

#endif

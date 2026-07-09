#include "script_runner.hpp"

namespace Game {
    namespace Scripts {
        ScriptRunner::ScriptRunner() : currentScript(nullptr) {
        }

        ScriptRunner::~ScriptRunner() {
        }

        void ScriptRunner::Start(std::shared_ptr<Script> script) {
            this->currentScript = script;

            this->actionRunner.Start(script->actions);
        }

        void ScriptRunner::Update(const float deltaTime) {
            this->actionRunner.Update(deltaTime);
        }

        bool ScriptRunner::IsCompleted() const {
            return this->actionRunner.IsCompleted();
        }
    }
}

#ifndef SDL03_Game_Scripts_ScriptRunner
#define SDL03_Game_Scripts_ScriptRunner

#include "../actions/action_runner.hpp"
#include "dialogue/dialogue_session.hpp"
#include "script.hpp"

namespace Game {
    namespace Scripts {
        class ScriptRunner {
        public:
            std::shared_ptr<Script> currentScript;
            Actions::ActionRunner actionRunner;
            Dialogue::DialogueSession dialogueSession;
            std::shared_ptr<ScriptNode> currentNode;
            std::shared_ptr<ScriptNode> pendingNode;

            ScriptRunner();
            ~ScriptRunner();
            void Start(std::shared_ptr<Script> script);
            void Update(const float deltaTime);
            void Render();
            bool IsCompleted() const;

            void AdvanceToNextStep();
            void JumpToNode(const std::string& nodeId);

        private:
            std::queue<std::shared_ptr<Steps::ScriptStep>> steps;
            std::shared_ptr<Steps::ScriptStep> currentStep;
            bool completed;

            void SetCurrentNode(std::shared_ptr<ScriptNode> node);
        };
    }
}

#endif

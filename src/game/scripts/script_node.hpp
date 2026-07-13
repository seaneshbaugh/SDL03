#ifndef SDL03_Game_Scripts_ScriptNode
#define SDL03_Game_Scripts_ScriptNode

#include "../../../../lib/nlohmann/json.hpp"

#include "../actions/action_parser.hpp"
#include "../services/locator.hpp"
#include "steps/action_step.hpp"
#include "steps/choice_step.hpp"
#include "steps/dialogue_step.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Scripts {
        class Script;

        class ScriptNode {
        public:
            std::shared_ptr<ScriptNode> next;
            std::vector<std::shared_ptr<Steps::ScriptStep>> steps;

            ScriptNode();
            ~ScriptNode();

        private:

        public:
            class Parser {
            public:
                Parser();
                ~Parser();
                std::shared_ptr<ScriptNode> ParseNode(const json& node, Script* script);

            private:
                using ScriptStepFactory = std::function<std::shared_ptr<Steps::ScriptStep>(const json& node, Script* script)>;

                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::unordered_map<std::string, ScriptStepFactory> scriptStepFactories;
            };
        };
    }
}

#endif

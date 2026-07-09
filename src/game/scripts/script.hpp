#ifndef SDL03_Game_Scripts_Script
#define SDL03_Game_Scripts_Script

#include "../../../../lib/nlohmann/json.hpp"

#include "../actions/action_parser.hpp"
#include "../interfaces/dialogue_manager.hpp"
#include "../scenes/scene.hpp"
#include "../services/locator.hpp"
#include "script_node.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Scripts {
        class Script {
        public:
            Scenes::Scene* scene;
            Interfaces::DialogueManager* dialogueManager;
            std::vector<std::shared_ptr<Actions::Base>> actions;
            std::map<std::string, std::shared_ptr<ScriptNode>> nodes;

            Script(Scenes::Scene* scene, Interfaces::DialogueManager* dialogueManager, const std::string& scriptId);
            ~Script();
            bool Load(const std::string& scriptId);

        private:
            std::string scriptId;
            bool ParseFile(const std::string& jsonString);

            class Parser {
            public:
                Parser();
                ~Parser();
                void Parse(const std::string& jsonString, Script* script);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
            };
        };
    }
}

#endif

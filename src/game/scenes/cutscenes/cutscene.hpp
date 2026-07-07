#ifndef SDL03_Game_Scene_Cutscenes_Cutscene
#define SDL03_Game_Scene_Cutscenes_Cutscene

#include "../../../../lib/nlohmann/json.hpp"

#include "../../services/locator.hpp"
#include "../scene.hpp"
#include "../../interfaces/dialogue_manager.hpp"
#include "../../actions/action_parser.hpp"
#include "../../objects/maps/spawn_point.hpp"
#include "../../../helpers/file_system.hpp"

using json = nlohmann::json;

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        namespace Cutscenes {
            class Cutscene {
            public:
                Cutscene(Scenes::Scene* scene, Interfaces::DialogueManager* dialogueManager, const std::string& cutsceneId);
                ~Cutscene();
                bool Load(const std::string& cutsceneId);

                Scenes::Scene* scene;
                Interfaces::DialogueManager* dialogueManager;
                std::vector<std::shared_ptr<Actions::Base>> actions;

            private:
                std::string cutsceneId;

                bool ParseFile(const std::string& jsonString);

                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, Cutscene* cutscene);

                private:
                    using ActionFactory = std::function<std::shared_ptr<Actions::Base>(const json& node, Cutscene* cutscene)>;

                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                    std::unordered_map<std::string, ActionFactory> actionFactories;

                    std::shared_ptr<Actions::Base> ParseAction(const json& node, Cutscene* cutscene);
                };
            };
        }
    }
}

#endif

#ifndef SDL03_Game_Scene_Cutscenes_Cutscene
#define SDL03_Game_Scene_Cutscenes_Cutscene

#include "../../../../lib/nlohmann/json.hpp"

#include "../../services/locator.hpp"
#include "actions/dialogue.hpp"
#include "actions/move_actor.hpp"
#include "actions/wait.hpp"
#include "../../../helpers/file_system.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scene {
        namespace Cutscenes {
            class Cutscene {
            public:
                Cutscene(States::Map* map, const std::string& cutsceneId);
                ~Cutscene();
                bool Load(const std::string& cutsceneId);

                std::vector<std::shared_ptr<Actions::Base>> actions;

            private:
                States::Map* map;
                std::string cutsceneId;

                bool ParseFile(const std::string& jsonString);

                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, Cutscene* cutscene);

                private:
                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                };
            };
        }
    }
}

#endif

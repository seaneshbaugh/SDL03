#ifndef SDL03_Game_Scenes_Dialogue_DialogueProfile
#define SDL03_Game_Scenes_Dialogue_DialogueProfile

#include <vector>

#include "../../../../lib/nlohmann/json.hpp"
#include "../../world/conditions/condition.hpp"
#include "../../../helpers/file_system.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Scenes {
        namespace Dialogue {
            class DialogueProfile {
            public:
                struct Entry {
                    std::unique_ptr<World::Conditions::Condition> condition;
                    std::string dialogueId;
                };

                std::vector<Entry> entries;

                DialogueProfile(const std::string& profileName);
                ~DialogueProfile();
                
            private:
                std::string profileName;

                bool Load();

                class Parser {
                public:
                    Parser();
                    ~Parser();

                    bool Parse(const std::string& jsonString, DialogueProfile* dialogueProfile);
                };
            };
        }
    }
}

#endif

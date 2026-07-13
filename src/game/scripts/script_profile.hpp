#ifndef SDL03_Game_Scripts_ScriptProfile
#define SDL03_Game_Scripts_ScriptProfile

#include <vector>

#include "../../../../lib/nlohmann/json.hpp"
#include "../world/conditions/condition.hpp"
#include "../../helpers/file_system.hpp"

using json = nlohmann::json;

namespace Game {
        namespace Scripts {
            class ScriptProfile {
            public:
                struct Entry {
                    std::unique_ptr<World::Conditions::Condition> condition;
                    std::string scriptId;
                };

                std::vector<Entry> entries;

                ScriptProfile(const std::string& profileName);
                ~ScriptProfile();
                
            private:
                std::string profileName;

                bool Load();

                class Parser {
                public:
                    Parser();
                    ~Parser();

                    bool Parse(const std::string& jsonString, ScriptProfile* scriptProfile);
                };
            };
        }
}

#endif

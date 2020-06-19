#ifndef SDL03_Game_Objects_Maps_MapEncounterArea
#define SDL03_Game_Objects_Maps_MapEncounterArea

#include <string>

#include "json.hpp"

#include "../../../helpers/file_system.hpp"
#include "../../../helpers/string.hpp"
#include "../../services/locator.hpp"
#include "map_object.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Objects {
        namespace Maps {
            class MapEncounterArea : public MapObject {
            public:
                // TODO: Consider extracting the inner vector to its own class which keeps
                // track of the mobs.
                std::vector<std::vector<std::string>> mobs;

                MapEncounterArea(const std::string& filename);
                ~MapEncounterArea();

            private:
                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, std::shared_ptr<MapEncounterArea> encounterArea);

                private:
                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                };

            public:
                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<sol::state> luaState);
                };
            };
        }
    }
}

#endif

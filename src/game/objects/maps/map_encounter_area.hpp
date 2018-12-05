#ifndef SDL03_Game_Objects_Maps_MapEncounterArea
#define SDL03_Game_Objects_Maps_MapEncounterArea

#include <string>

#include "libjson.h"

#include "../../../helpers/file_system.hpp"
#include "../../services/locator.hpp"
#include "map_object.hpp"

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
            };
        }
    }
}

#endif

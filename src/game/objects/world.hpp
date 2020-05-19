#ifndef SDL03_Game_Objects_World
#define SDL03_Game_Objects_World

#include "../services/locator.hpp"

#include "party.hpp"
#include "monster.hpp"
#include "map.hpp"

namespace Game {
    namespace Objects {
        class World : public Base {
        public:
            static const std::string defaultMap;

            World();
            ~World();
            void SetStartingPlayerParty();
            void UnloadPlayerParty();
            // TODO: Make this take a mob list whenever I get around to making that a class.
            void SetEnemyParty(std::shared_ptr<Objects::Maps::MapEncounterArea> encounterArea);
            void UnloadEnemyParty();
            void LoadMap(const std::string& filename);
            void UnloadMap();

            std::shared_ptr<Objects::Characters::Party> playerParty;
            std::shared_ptr<Objects::Characters::Party> enemyParty;
            std::shared_ptr<Objects::Maps::Map> currentMap;
        };
    }
}

#endif

#ifndef SDL03_Game_Objects_World
#define SDL03_Game_Objects_World

#include "../../../lib/nlohmann/json.hpp"

#include "characters/party.hpp"
#include "characters/player_character.hpp"
#include "characters/monster.hpp"
#include "maps/map.hpp"

using json = nlohmann::json;

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
            void SetEnemyParty(const Objects::Maps::MapEncounterArea& encounterArea);
            void UnloadEnemyParty();
            void LoadMap(const std::string& filename);
            void UnloadMap();
            json AsJSON();

            std::shared_ptr<Objects::Characters::Party> playerParty;
            std::shared_ptr<Objects::Characters::Party> enemyParty;
            std::shared_ptr<Objects::Maps::Map> currentMap;
            unsigned int playerCurrentX;
            unsigned int playerCurrentY;
        };
    }
}

#endif

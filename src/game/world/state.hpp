#ifndef SDL03_Game_World_State
#define SDL03_Game_World_State

#include "../../../lib/nlohmann/json.hpp"

#include "flags.hpp"
#include "../objects/characters/party.hpp"
#include "../objects/characters/player_character.hpp"
#include "../objects/characters/monster.hpp"
#include "../objects/maps/map.hpp"

using json = nlohmann::json;

namespace Game {
    namespace World {
        class State {
        public:
            State();
            ~State();
            void SetStartingPlayerParty();
            void UnloadPlayerParty();
            // TODO: Make this take a mob list whenever I get around to making that a class.
            void SetEnemyParty(const Objects::Maps::MapEncounterArea& encounterArea);
            void UnloadEnemyParty();
            void LoadMap(const std::string& mapName);
            void UnloadMap();
            json AsJSON();

            Flags flags;
            std::shared_ptr<Objects::Characters::Party> playerParty;
            std::shared_ptr<Objects::Characters::Party> enemyParty;
            std::shared_ptr<Objects::Maps::Map> currentMap;
            unsigned int playerCurrentX;
            unsigned int playerCurrentY;

        private:
            std::shared_ptr<Log::Logger> logger;
        };
    }
}

#endif

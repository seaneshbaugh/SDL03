#ifndef SDL03_Game_States_Battle
#define SDL03_Game_States_Battle

#include <algorithm>

#include "base.hpp"
#include "../objects/image.hpp"
#include "../objects/characters/player_character.hpp"
#include "../objects/characters/monster.hpp"
#include "../objects/maps/map_encounter_area.hpp"
#include "../objects/world.hpp"

namespace Game {
    namespace States {
        class Battle : public Base {
        public:
            friend class MapState;

            std::shared_ptr<Objects::Maps::MapEncounterArea> encounterArea;

            Battle(const Objects::Maps::MapEncounterArea* encounterArea);
            ~Battle();
            void HandleEvent(const SDL_Event& event);
            std::shared_ptr<Base> Update();
            void Render();
            std::vector<std::shared_ptr<Objects::Characters::PlayerCharacter>> GetParty();
            std::vector<std::shared_ptr<Objects::Characters::Monster>> GetMonsters();

        private:
            static const std::string logChannel;

            std::string backgroundName;
            std::shared_ptr<Resources::Texture> background;

            std::string ProcessInput(const InputKey key);
            void LoadLuaState(const std::string& scriptFilePath);

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif

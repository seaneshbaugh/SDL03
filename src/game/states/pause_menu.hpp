#ifndef SDL03_Game_States_PauseMenu
#define SDL03_Game_States_PauseMenu

#include "base.hpp"
#include "save_game_menu.hpp"
#include "../objects/image.hpp"
#include "../objects/characters/player_character.hpp"
#include "../objects/items/consumable.hpp"
#include "../objects/items/equippable.hpp"
#include "../objects/world.hpp"

namespace Game {
    namespace States {
        class PauseMenu : public Base {
        public:
            PauseMenu();
            ~PauseMenu();
            std::shared_ptr<Base> Update(const int key);
            std::shared_ptr<Base> Update(const SDL_Event& event);
            std::string ProcessInput(const int key);
            void Render();
            Objects::Characters::Party GetParty();
            std::vector<std::shared_ptr<Objects::Characters::PlayerCharacter>> GetPartyCharacters();
            std::tuple<unsigned int, unsigned int, unsigned int> GetClockTime();

        private:
            static const std::string logChannel;

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

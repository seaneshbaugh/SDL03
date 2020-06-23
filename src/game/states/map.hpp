#ifndef SDL03_Game_States_Map
#define SDL03_Game_States_Map

#include "base.hpp"
#include "pause_menu.hpp"
#include "battle.hpp"
#include "../objects/maps/map.hpp"

namespace Game {
    namespace States {
        class Map : public Base {
        public:
            Map();
            ~Map();
            std::shared_ptr<Base> Update(const int key);
            std::shared_ptr<Base> Update(const SDL_Event& event);
            std::string ProcessInput(const int key);
            void Render();
            bool LoadMap(const std::string& filename);
            bool UnloadMap();
            std::shared_ptr<Objects::Maps::Map> GetCurrentMap();
            std::shared_ptr<Objects::Maps::MapEncounterArea> GetCurrentMapEncounterArea(const int x, const int y);
            void SetCurrentMapEncounterArea(Objects::Maps::MapObject* mapEncounterArea);
            std::string GetPlayerSpriteName();
            void Step(unsigned int x, unsigned int y);

        private:
            static const std::string logChannel;

            std::shared_ptr<Objects::Maps::Map> currentMap;
            Objects::Maps::MapEncounterArea* currentMapEncounterArea;

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

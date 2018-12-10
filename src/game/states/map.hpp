#ifndef SDL03_Game_States_Map
#define SDL03_Game_States_Map

#include "base.hpp"
#include "battle.hpp"
#include "../objects/maps/map.hpp"

namespace Game {
    namespace States {
        class Map : public Base {
        public:
            std::shared_ptr<Objects::Maps::Map> currentMap;

            Map();
            ~Map();
            std::shared_ptr<Base> Update(const int key);
            std::shared_ptr<Base> Update(const SDL_Event& event);
            std::string ProcessInput(const int key);
            void Render();
            bool LoadMap(const std::string& filename);
            bool UnloadMap();

        private:
            static const std::string logChannel;

            std::shared_ptr<Objects::Maps::MapEncounterArea> currentMapEncounterArea;

            void LoadLuaContext(const std::string& scriptFile);

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
            };
        };
    }
}



#endif
